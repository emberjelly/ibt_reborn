/*
	ibt_frontend.cpp

	A front end to the ibt beat tracker.

	example usage:

	./ibt -i input.wav -o output.txt
	
*/

#include "ibt.h"
#include <iostream>
#include "sndfile.h"
#include <string>
#include <fstream>
#include "drum_machine.h"
#include <stdlib.h>

#define BLOCK_SIZE 512
#define SAMPLE_RATE 44100.0

void print_help()
{
	std::cout << "Usage: ./ibt [-i input_wav] [-o output_txt] [-m] [--testsuite]" << std::endl;
	std::cout << "Options..." << std::endl << std::endl;

	std::cout << "\t-i\t\tInput wav file for beat tracking" << std::endl;
	std::cout << "\t-o\t\tOutput text file for saving beats" << std::endl;
	std::cout << "\t-m\t\tUse the microphone for input" << std::endl;
	std::cout << "\t--drums\tWrite beats to the hardware over wiringPi (will only work on embedded system)" << std::endl;
	std::cout << std::endl;
}

int main(int argc, const char **argv)
{

	std::string out_file_name = "out_beat_times.txt";
	std::string in_file_name = "music.wav";
	bool mic_input = false;
	bool drum_output = false;

	if (argc == 1)
	{
		print_help();
		return 0;
	}

	for (auto arg = argv + 1; arg != argv + argc; ++arg)
	{
		std::string argstr = *arg;
		
		if (argstr == "-i")
		{
			// Read the name of the input file
			++arg;
			in_file_name = *arg;
			std::cout << "Reading from: " << in_file_name << std::endl;
		}

		else if (argstr == "-o")
		{
			// Read the name of the input file
			++arg;
			out_file_name = *arg;
			std::cout << "Writing to: " << out_file_name << std::endl;

		}

		else if (argstr == "-m")
		{
			std::cout << "Running in mic input mode" << std::endl;
			mic_input = true;
		}
		else if (argstr == "--drums")
		{
			drum_output = true;
		}
		else
		{
			print_help();
			return 0;
		}
	}

	if (!mic_input)
	{

		SF_INFO sfinfo;
		sfinfo.samplerate = 44100;
		sfinfo.format = SF_FORMAT_WAV;
		sfinfo.channels = 1;
		double frame[BLOCK_SIZE];
		const char *options[] = {"tmp", "-o", "beats"};
		IBTSystem ibt_sys(options, 3);
		SNDFILE* sndfile = sf_open(in_file_name.c_str(), SFM_READ, &sfinfo);

		FILE *fp;
		fp = fopen(out_file_name.c_str(), "w");
		sf_count_t c = 1;
		unsigned int block_count = 0;
		while(c != 0)
		{
			c = sf_readf_double(sndfile, frame, BLOCK_SIZE);
			
			if (ibt_sys.ibt(frame))
			{
				std::cout << "Found beat: " << block_count*BLOCK_SIZE/SAMPLE_RATE << std::endl;
				fprintf(fp, "%f\n", block_count*BLOCK_SIZE/SAMPLE_RATE);
			}

			++block_count;
		}
		fclose(fp);
	} else {

		if (drum_output)
		{
			DrumMachine d;
			const char *options[] = {"tmp", "-mic", "-o", "beats"};
			IBTSystem ibt_sys(options, 4);
			FILE *fp;
			fp = fopen(out_file_name.c_str(), "w");

			unsigned int block_count = 0;			
			while(true)
			{
				bool beat = ibt_sys.ibt(nullptr);
				if (beat)
				{
					std::cout << "Beat: " << block_count*BLOCK_SIZE/SAMPLE_RATE << std::endl;
					fprintf(fp, "%f\n", block_count*BLOCK_SIZE/SAMPLE_RATE);
				}
				d.update(beat);
				++block_count;
			}
			fclose(fp);

		} else {
			const char *options[] = {"tmp", "-mic", "-o", "beats"};
			IBTSystem ibt_sys(options, 4);
			FILE *fp;
			fp = fopen(out_file_name.c_str(), "w");

			unsigned int block_count = 0;			
			while(true)
			{
				if (ibt_sys.ibt(nullptr))
				{
					std::cout << "Beat: " << block_count*BLOCK_SIZE/SAMPLE_RATE << std::endl;
					fprintf(fp, "%f\n", block_count*BLOCK_SIZE/SAMPLE_RATE);
				}

				++block_count;
			}
			fclose(fp);
		}
	}
}

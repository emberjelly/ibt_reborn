#ifndef DRUM_MACHINE
#define DRUM_MACHINE

#include <iostream>
#include <thread>
#include <unistd.h>
#include <cmath>

#define GPIO

#ifdef GPIO
//#include "../src/serial/serial.h"
#include "wiringPi.h"
#endif

#define LED 0
#define FRAME_LEN_SAMPLES 	512.0
#define SAMPLING_RATE 		44100.0
#define FRAME_LEN_TIME 		(FRAME_LEN_SAMPLES/SAMPLING_RATE)



class DrumMachine {
public:
	

	DrumMachine(const float _delay = 0.01) : delay{_delay},
											beat_divs{1},
											beat_divs_inv{1/4.0f},
											curr_frame{0},
											last_beat_frame{0},
											curr_beat_frame{0},
											interval{0},
											sub_interval{0},
											sub_interval_steps{0}
	{
		// Initialise GPIO if we are using the raspberry pi									
		#ifdef GPIO										
		wiringPiSetup();
  		pinMode(LED, OUTPUT);
		#endif	
	}


	void update(bool is_beat) 
	{


		if (is_beat)
		{
			last_beat_frame = curr_beat_frame;

			curr_beat_frame = curr_frame;
			
			
			last_last_interval = last_interval;
			last_interval = cur_interval;
			cur_interval = ((curr_beat_frame - last_beat_frame));
			
			if(last_last_interval == 0)
			{
				interval = ((curr_beat_frame - last_beat_frame));
			} else {
				
				interval = std::round((cur_interval + last_interval + last_last_interval) / 3.0);

			}
			

			//interval = ((curr_beat_frame - last_beat_frame));

			sub_interval = std::round(interval / beat_divs);
			
			sub_interval_steps = 0;
			sub_interval_count = 0;

			if (curr_frame*FRAME_LEN_TIME > 10)
			{
			#ifdef GPIO
				std::thread ledt([&] () {
					usleep(1000000*interval*FRAME_LEN_TIME - 35000);
					digitalWrite(LED,false); // The dum machine is active low
					usleep(10000);
					digitalWrite(LED,true);
				});
				ledt.detach();
			#endif
			std::cout << "beat" << 60.0/(interval*FRAME_LEN_TIME) << std::endl;
			}

		}else{
			if (sub_interval_count != beat_divs-1)
			{
				if (sub_interval_steps == sub_interval)
				{
					#ifdef GPIO
					std::thread ledt([&] () {
			//			usleep(1000000*interval*FRAME_LEN_TIME - 50000);
						digitalWrite(LED,false); // The dum machine is active low
						usleep(10000);
						digitalWrite(LED,true);
					});
					ledt.detach();
					#endif

					sub_interval_steps = 0;
					sub_interval_count ++;
				}

				sub_interval_steps += 1;
			}
		}
		curr_frame++;		
	}

	
private:
	
	const float delay;				// Experimentally measured time taken to hit the drum
	const unsigned int beat_divs;
	const float beat_divs_inv;
	unsigned int curr_frame;
	unsigned int last_beat_frame;
	unsigned int curr_beat_frame;
	unsigned int interval;
	unsigned int sub_interval;
	unsigned int sub_interval_steps;
	unsigned int cur_interval = 0;
	unsigned int last_interval = 0;
	unsigned int last_last_interval = 0;
	unsigned int sub_interval_count = 0;
};


#endif

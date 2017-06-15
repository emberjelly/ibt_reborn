#ifndef IBT_H
#define IBT_H

/*
** Copyright (C) 2000-2010 George Tzanetakis <gtzan@cs.uvic.ca>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

/*
** IBT - standing for INESC-Porto Beat Tracker - is a real-time/off-line
** tempo induction and beat tracking system based on a competing multi-agent
** strategy, which considers parallel hypotheses regarding tempo and beats.
**
** Published in:
** Oliveira J. L., Gouyon F., Martins L., Reis L. P. IBT: A Real-Time Tempo and Beat Tracking System.
** International Conference on Music Information Retrieval, pp. 291- 296, Utrecht, 2010. ISBN: 978-90-393-53813.
**
*/

#include <cstdio>
#include <cstdlib>
#include <string>
#include <iomanip>

#include <marsyas/common_source.h>
#include <marsyas/CommandLineOptions.h>
#include <marsyas/Collection.h>
#include <marsyas/FileName.h>
#include <marsyas/NumericLib.h>
#include <marsyas/system/MarSystemManager.h>
#include <marsyas/sched/EvValUpd.h>
#include <marsyas/marsystems/MarSystemTemplateBasic.h>
#include <marsyas/marsystems/MarSystemTemplateAdvanced.h>

#include <string.h>

#include <sndfile.h>



//#define VEC_INPUT

#ifdef MARSYAS_MIDIIO
#include "RtMidi.h"
#endif

//#ifdef MARSYAS_AUDIOIO
//#include "RtAudio3.h"
//#endif

using namespace std;
using namespace Marsyas;

#ifdef MARSYAS_WIN32
#pragma warning(disable: 4244)  //disable double to float warning
#pragma warning(disable: 4100) //disable argc warning
#endif

//mrs_bool sonicOutFlux = 0;
//mrs_bool sonicOutFluxFilter = 0;

//============================== IBT FUNCTIONAL PARAMETERS ==============================

#define BPM_HYPOTHESES 6 //Nr. of initial BPM hypotheses (must be <= than the nr. of agents) (6)
#define PHASE_HYPOTHESES 30//Nr. of phases per BPM hypothesis (30)
#define MIN_BPM 81 //minimum tempo considered, in BPMs (50) [81 -> to prevent octave error]
#define MAX_BPM 160 //maximum tempo considered, in BPMs (250) [160 -> to prevent octave error]
#define NR_AGENTS 30 //Nr. of agents in the pool (30)
#define LFT_OUTTER_MARGIN 0.20 //(Inertia1.1) The size of the outer half-window (in % of the IBI) before the predicted beat time (0.20)
#define RGT_OUTTER_MARGIN 0.40 //(Inertia1.2) The size of the outer half-window (in % of the IBI) after the predicted beat time (0.30)
#define INNER_MARGIN 4.0 //(Inertia1.3) Inner tolerance window margin size (= half inner window size -> in ticks) (4.0)
#define OBSOLETE_FACTOR 0.8 //An agent is killed if, at any time (after the initial Xsecs-defined in BeatReferee), the difference between its score and the bestScore is below OBSOLETE_FACTOR * bestScore (0.8)
#define LOST_FACTOR 8 //An agent is killed if it become lost, i.e. if it found LOST_FACTOR consecutive beat predictions outside its inner tolerance window (8)
#define CHILDREN_SCORE_FACTOR 0.9 //(Inertia2) Each created agent imports its father score multiplied (or divided if negative) by this factor (0.8)
#define BEST_FACTOR 1.0 //(Inertia3) Mutiple of the bestScore an agent's score must have for replacing the current best agent (1.0)
#define CORRECTION_FACTOR 0.25 //(Inertia4) correction factor for compensating each agents' own {phase, period} hypothesis errors (0.25)
#define EQ_PERIOD 1 //Period threshold which identifies two agents as predicting the same period (IBI, in ticks) (1)
#define EQ_PHASE 2 //Phase threshold which identifies two agents as predicting the same phase (phase, in ticks) (2)
#define CHILD1_FACTOR 1.0 //Correction factor (error proportion-[0.0-1.0]) for compensating its father's {phase, period} hypothesis - used by child1 (2.0 - only full phase adjustment; -1 - no child considered) (1.0)
#define CHILD2_FACTOR 2.0 //Correction factor (error proportion-[0.0-1.0]) for compensating its father's {phase, period} hypothesis - used by child2 (2.0 - only full phase adjustment; -1 - no child considered) (2.0)
#define CHILD3_FACTOR 0.5 //Correction factor (error proportion-[0.0-1.0]) for compensating its father's {phase, period} hypothesis - used by child3 (2.0 - only full phase adjustment; -1 - no child considered) (0.5)
#define TRIGGER_GT_TOL 5 //Number of miss computed beats, in comparison to ground-truth beat-times, tolerated before triggering new induction (used in trigger "groundtruth" mode) -> can be defined via -tigt_tol 
#define TRIGGER_BEST_FACTOR 1.0 //Proportion of the current best agent score which is inherited by the agents created at each triggered induction [shouldn't be much higher than 1, for not inflating scores two much] (1.0)
#define SUPERVISED_TRIGGER_THRES 0.03 //Degree (in percentage) of mean bestScore decrease to trigger a new induction in supervised induction mode (0.03)
#define BEAT_TRANSITION_TOL 0.6 //Tolerance for handling beats at transitions between agents [-1 for unconsider it]: (0.6)
//In causal mode, if between two consecutive beats there is over a BEAT_TRANSITION_TOL decrease in current IBI the second beat is unconsidered;
//In non-causal mode, if between a son's first beat and its father's last there is over a BEAT_TRANSITION_TOL descrease on the father last IBI the son's first beat is unconsidered;
//In non-causal mode, if between a son's first beat and its father's last there is over a BEAT_TRANSITION_TOL increase on the father last IBI the son's first beat shall be its father's next beat, and the second beat shall be its assigned first.

#define WINSIZE 1024 //(2048?)
#define HOPSIZE 512 //(512)

//=======================================================================================

class IBTSystem
{
public:

	IBTSystem(const char* options[], const int num_options);

	// Constructor
	//IBTSystem(mrs_string sfName, mrs_string outputTxt);
	
	~IBTSystem()
	{
		delete audioflow;
		remove( "tmp.wav" );
		remove("tmp.txt");
	}

	bool ibt(double *frame);
	void init(mrs_string sfName, mrs_string outputTxt);

	// All the variables that need to remain for the duration of the ibt function.
	MarSystemManager mng;

	// These are all the systems
	MarSystem* audioflow;
	MarSystem* onsetdetectionfunction;
	MarSystem* beattracker;
	MarSystem* normfiltering;
	MarSystem* tempoinduction;
	MarSystem* tempohypotheses;
	MarSystem* tempo;
	MarSystem* phase;
	MarSystem* initialhypotheses;
	MarSystem* agentpool;
	MarSystem* IBTsystem;
	MarSystem* beatmix;
	MarSystem* beatsynth;

	mrs_string sfName;
	mrs_string outputTxt;
	
	mrs_natural inputSize;
	mrs_natural inductionTickCount;

	mrs_realvec stored_data{2048};

	unsigned int frameCount = 0;

	SNDFILE* ibtfile;
	SF_INFO sfinfo;
	int curr_file_size = 0;

};




#endif
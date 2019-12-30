/**
 * @file midi.h
 *
 * Handling signal frames.
 *
 * This file is part of the ispeech library.
 * ispeech is free software, licensed under the MIT/X11 License. A copy of
 * the license is provided with the library in the LICENSE file.
 *
 * @package vfx_ispeech
 * @version 0.0.1-dev
 * @author Han Zhongtao
 * @date 2019-2019
 * @license
 * @since 0.0.1
 */

#ifndef MIDI_H
#define MIDI_H

#include "../../ispeech/global.h"
#include "../../3rdparty/midifile/include/MidiFile.h"
#include "../../3rdparty/midifile/include/Options.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <vector>

#define TICK 1
#define BEAT 2
#define SEC  3
#define MSEC 4

#define OP_NOTE    1000
#define OP_CONTROL 2000
#define OP_INSTR   3000
#define OP_TEMPO   4000
#define OP_METER   5000
#define OP_KEYSIG  6000

using namespace std;
using namespace smf;

namespace vfx_ispeech
{
    /**
     * An ecapsulation of a single frame of the signal.
     *
     * The Frame class wraps a signal frame (short fragment of a signal).
     * Frame itself can be considered as a signal source, being a "window"
     * over original signal data. It is a lightweight object which can be
     * copied by value. No data are copied - only the pointer to source
     * and frame boundaries.
     *
     * Frame samples are accessed by STL-compatible iterators, as is the
     * case with all SignalSource-derived classes. Frame sample number N
     * is the same as sample number FRAME_BEGIN+N in the original source.
     *
     * Example (source size = N, frame size = M, frame starts at 8th sample):
     *
     * @verbatim
     * sample number:          0       8                                       N
     * original source:        xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
     * frame:                          |xxxxxxxxxxxx|
     * sample number in frame:         0             M              @endverbatim
     */
    class ISPEECH_EXPORT midi
    {
    public:
	midi();

	/**
	 * Returns the frame length.
	 *
	 * @return frame length as a number of samples
	 */

	int take(int argc, char* argv[]);
    public:





	const char *GMinstrument[128] = {
					 "acpiano",   "britepno",  "synpiano",  "honkytonk", "epiano1",   "epiano2",
					 "hrpschrd",  "clavinet",  "celeste",   "glocken",   "musicbox",  "vibes",
					 "marimba",   "xylophon",  "tubebell",  "santur",    "homeorg",   "percorg",
					 "rockorg",   "churchorg", "reedorg",   "accordn",   "harmonica", "concrtna",
					 "nyguitar",  "acguitar",  "jazzgtr",   "cleangtr",  "mutegtr",   "odguitar",
					 "distgtr",   "gtrharm",   "acbass",    "fngrbass",  "pickbass",  "fretless",
					 "slapbas1",  "slapbas2",  "synbass1",  "synbass2",  "violin",    "viola",
					 "cello",     "contraba",  "marcato",   "pizzcato",  "harp",      "timpani",
					 "marcato",   "slowstr",   "synstr1",   "synstr2",   "choir",     "doo",
					 "voices",    "orchhit",   "trumpet",   "trombone",  "tuba",      "mutetrum",
					 "frenchorn", "hitbrass",  "synbras1",  "synbras2",  "sprnosax",  "altosax",
					 "tenorsax",  "barisax",   "oboe",      "englhorn",  "bassoon",   "clarinet",
					 "piccolo",   "flute",     "recorder",  "woodflut",  "bottle",    "shakazul",
					 "whistle",   "ocarina",   "sqrwave",   "sawwave",   "calliope",  "chiflead",
					 "charang",   "voxlead",   "lead5th",   "basslead",  "fantasia",  "warmpad",
					 "polysyn",   "ghostie",   "bowglass",  "metalpad",  "halopad",   "sweeper",
					 "aurora",    "soundtrk",  "crystal",   "atmosphr",  "freshair",  "unicorn",
					 "sweeper",   "startrak",  "sitar",     "banjo",     "shamisen",  "koto",
					 "kalimba",   "bagpipes",  "fiddle",    "shannai",   "carillon",  "agogo",
					 "steeldrum", "woodblock", "taiko",     "toms",      "syntom",    "revcymb",
					 "fx-fret",   "fx-blow",   "seashore",  "jungle",    "telephone", "helicptr",
					 "applause",  "ringwhsl"
	};

	const char *GMcontrollers[128] = {
					  "  0   Bank Select (coarse)               0..127",
					  "  1   Modulation Wheel (coarse)          0..127",
					  "  2   Breath Control (coarse)            0..127",
					  "  3   Continuous controller #3           0..127",
					  "  4   Foot Controller (coarse)           0..127",
					  "  5   Portamento Time (coarse)           0..127",
					  "  6   Data Entry Slider (coarse)         0..127",
					  "  7   Main Volume (coarse)               0..127",
					  "  8   Stereo Balance (coarse)            0..127",
					  "  9   Continuous controller #9           0..127",
					  " 10   Pan (coarse)                       0=left 127=right",
					  " 11   Expression (sub-Volume) (coarse)   0..127",
					  " 12   Effect Control 1 (coarse)          0..127",
					  " 13   Effect Control 2 (coarse)          0..127",
					  " 14   Continuous controller #14          0..127",
					  " 15   Continuous controller #15          0..127",
					  " 16   General Purpose Slider 1           0..127",
					  " 17   General Purpose Slider 2           0..127",
					  " 18   General Purpose Slider 3           0..127",
					  " 19   General Purpose Slider 4           0..127",
					  " 20   Continuous controller #20          0..127",
					  " 21   Continuous controller #21          0..127",
					  " 22   Continuous controller #22          0..127",
					  " 23   Continuous controller #23          0..127",
					  " 24   Continuous controller #24          0..127",
					  " 25   Continuous controller #25          0..127",
					  " 26   Continuous controller #26          0..127",
					  " 27   Continuous controller #27          0..127",
					  " 28   Continuous controller #28          0..127",
					  " 29   Continuous controller #29          0..127",
					  " 30   Continuous controller #30          0..127",
					  " 31   Continuous controller #31          0..127",
					  " 32   Bank Select (fine)                 0..127 usu.ignored",
					  " 33   Modulation Wheel (fine)            0..127",
					  " 34   Breath Control (fine)              0..127",
					  " 35   Continuous controller #3 (fine)    0..127",
					  " 36   Foot Controller (fine)             0..127",
					  " 37   Portamento Time (fine)             0..127",
					  " 38   Data Entry Slider (fine)           0..127",
					  " 39   Main Volume (fine)                 0..127 usu. ignored",
					  " 40   Stereo Balance (fine)              0..127",
					  " 41   Continuous controller #9 (fine)    0..127",
					  " 42   Pan (fine)                         0..127 usu. ignored",
					  " 43   Expression (sub-Volume) (fine)     0..127 usu. ignored",
					  " 44   Effect Control 1 (fine)            0..127",
					  " 45   Effect Control 2 (fine)            0..127",
					  " 46   Continuous controller #14 (fine)   0..127",
					  " 47   Continuous controller #15 (fine)   0..127",
					  " 48   Continuous controller #16          0..127",
					  " 49   Continuous controller #17          0..127",
					  " 50   Continuous controller #18          0..127",
					  " 51   Continuous controller #19          0..127",
					  " 52   Continuous controller #20 (fine)   0..127",
					  " 53   Continuous controller #21 (fine)   0..127",
					  " 54   Continuous controller #22 (fine)   0..127",
					  " 55   Continuous controller #23 (fine)   0..127",
					  " 56   Continuous controller #24 (fine)   0..127",
					  " 57   Continuous controller #25 (fine)   0..127",
					  " 58   Continuous controller #26 (fine)   0..127",
					  " 59   Continuous controller #27 (fine)   0..127",
					  " 60   Continuous controller #28 (fine)   0..127",
					  " 61   Continuous controller #29 (fine)   0..127",
					  " 62   Continuous controller #30 (fine)   0..127",
					  " 63   Continuous controller #31 (fine)   0..127",
					  " 64   Hold pedal (Sustain) on/off        0..63=off  64..127=on",
					  " 65   Portamento on/off                  0..63=off  64..127=on",
					  " 66   Sustenuto Pedal on/off             0..63=off  64..127=on",
					  " 67   Soft Pedal on/off                  0..63=off  64..127=on",
					  " 68   Legato Pedal on/off                0..63=off  64..127=on",
					  " 69   Hold Pedal 2 on/off                0..63=off  64..127=on",
					  " 70   Sound Variation                    0..127",
					  " 71   Sound Timbre                       0..127",
					  " 72   Sound Release Time                 0..127",
					  " 73   Sound Attack Time                  0..127",
					  " 74   Sound Brighness                    0..127",
					  " 75   Sound Control 6                    0..127",
					  " 76   Sound Control 7                    0..127",
					  " 77   Sound Control 8                    0..127",
					  " 78   Sound Control 9                    0..127",
					  " 79   Sound Control 10                   0..127",
					  " 80   General Purpose Button             0..63=off 64..127=on",
					  " 81   General Purpose Button             0..63=off 64..127=on",
					  " 82   General Purpose Button             0..63=off 64..127=on",
					  " 83   General Purpose Button             0..63=off 64..127=on",
					  " 84   Undefined on/off                   0..63=off 64..127=on",
					  " 85   Undefined on/off                   0..63=off 64..127=on",
					  " 86   Undefined on/off                   0..63=off 64..127=on",
					  " 87   Undefined on/off                   0..63=off 64..127=on",
					  " 88   Undefined on/off                   0..63=off 64..127=on",
					  " 89   Undefined on/off                   0..63=off 64..127=on",
					  " 90   Undefined on/off                   0..63=off 64..127=on",
					  " 91   Effects Level                      0..127",
					  " 92   Tremulo Level                      0..127",
					  " 93   Chorus Level                       0..127",
					  " 94   Celeste (Detune) Level             0..127",
					  " 95   Phaser Level                       0..127",
					  " 96   Data entry +1                      ignored",
					  " 97   Data entry -1                      ignored",
					  " 98   Non-Registered Parameter Number (coarse)0..127",
					  " 99   Non-Registered Parameter Number (fine)  0..127",
					  "100   Registered Parameter Number (coarse)    0..127",
					  "101   Registered Parameter Number (fine) 0..127",
					  "102   Undefined                          ?",
					  "103   Undefined                          ?",
					  "104   Undefined                          ?",
					  "105   Undefined                          ?",
					  "106   Undefined                          ?",
					  "107   Undefined                          ?",
					  "108   Undefined                          ?",
					  "109   Undefined                          ?",
					  "110   Undefined                          ?",
					  "111   Undefined                          ?",
					  "112   Undefined                          ?",
					  "113   Undefined                          ?",
					  "114   Undefined                          ?",
					  "115   Undefined                          ?",
					  "116   Undefined                          ?",
					  "117   Undefined                          ?",
					  "118   Undefined                          ?",
					  "119   Undefined                          ?",
					  "120   All Sound Off                      ignored",
					  "121   All Controllers Off                ignored",
					  "122   Local Keyboard On/Off              0..63=off 64..127=on",
					  "123   All Notes Off                      ignored",
					  "124   Omni Mode Off                      ignored",
					  "125   Omni Mode On                       ignored",
					  "126   Monophonic Mode On                 **",
					  "127   Polyphonic Mode On (mono=off)      ignored"
	};


	vector<int> legend_instr;
	vector<int> legend_opcode;
	vector<int> legend_controller;

	// user interface variables
	Options options;
	int     debugQ   = 0;           // use with --debug option
	int     verboseQ = 0;           // used with -v option
	int     tickQ    = 0;           // used with -t option
	int     beatQ    = 0;           // used with -b option
	int     secQ     = 0;           // used with -s option
	int     msecQ    = 0;           // used with -m option
	double  unused   = -1000.0;     // used with -u option
	char    arrayname[1024] = {0};  // used with -n option
	int     timetype = SEC;
	double  tempo    = 60.0;
	int     maxcount = 100000;
	vector<vector<double> > matlabarray;
	vector<int> channelfilter;

	// function declarations:
	void      convertMidiFile       (MidiFile& midifile,
					 vector<vector<double> >& matlab);
	void      setTempo              (MidiFile& midifile, int index, double& tempo);
	void      checkOptions          (Options& opts, int argc, char** argv);
	void      example               (void);
	void      usage                 (const char* command);
	double    getTime               (int ticks, double tempo, int tpq);
	void      processMetaEvent      (MidiFile& midifile, int i,
					 vector<double>& event);
	void      printEvent            (vector<double>& event);
	void      printLegend           (MidiFile& midifile);
	void      printMatlabArray      (MidiFile& midifile,
					 vector<vector<double> >& matlab);
	void      sortArray             (vector<vector<double> >& matlab);


	void      printNotesData       (MidiFile& midifile,
					vector<vector<double> >& matlab);
	void      printNotesEvent      (vector<double>& event);
	void      setFilterOptions     (vector<int>& channelfilter, const char* exclude);

    };
}

#endif // MIDI_H

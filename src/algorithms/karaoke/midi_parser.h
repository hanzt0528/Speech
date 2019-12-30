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

#ifndef MIDI_PSARSER_H
#define MIDI_PSARSER_H

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
     * convert midi file to pitch array.
     */
    class ISPEECH_EXPORT midi_parser
    {
    public:
	midi_parser();

    /**
     *  Parser the midi file.
     *
     * @Param midi file of midi
     * @Param pitch_array it save the all pitchs of parsed.
     */
    int parser(char* midi,float hop_size_in_ms,vector<MIDIDescriptorType> &pitch_array);
    vector<MIDIDescriptorType> parser2(char* midi,float hop_size_in_ms);
    };

  class NoteItem {
  public:
    NoteItem(double start, double end, double note):
      start(start), end(end), note(note) {

    }

    double start = 0;
    double end = 0;
    double note = 0;
  };

}

#endif // MIDI_PSARSER_H

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

#ifndef MIDI_MAKE_H
#define MIDI_MAKE_H

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

using namespace std;
using namespace smf;

namespace vfx_ispeech
{
    /**
     * Make a new  midi file from some melody and rhythm.
     */
    class ISPEECH_EXPORT midi_make
    {
    public:
	midi_make();

    /**
     *  Make a midi file.
     *
     * @Param midi file of midi
     * @Param melody input a lot of melody
     * @Param rhythm input a lot of rhythm
     * @Param tpq TicksPerQuarterNote
     */
    int make(char* midi,vector<double> &melody,vector<double> &rhythm,double tpq = 120);
    };
}

#endif // MIDI_MAKE_H

/**
 * @file karaoke.h
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

#ifndef KARAOKE_H
#define KARAOKE_H

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
    class ISPEECH_EXPORT karaoke
    {
    public:
	karaoke();

    /**
     *  Make a midi file.
     *
     * @Param midi file of midi
     * @Param melody input a lot of melody
     * @Param rhythm input a lot of rhythm
     * @Param tpq TicksPerQuarterNote
     * @Param threshold midi and wav,to tell  diference.
     */
    int set(std::string  midi,std::string wav,double hopsize_ms,double threshold=23.0);

    std::vector<MIDIDescriptorType> getmidi();
    std::vector<SoundDescriptorType> getwav();
    double getscore();

    std::vector<MIDIDescriptorType> midi_;
    std::vector<SoundDescriptorType> wav_;
    double score_;
    };
}

#endif // KARAOKE_H

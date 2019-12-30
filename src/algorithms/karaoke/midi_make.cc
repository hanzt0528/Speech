/**
 * @file midi_make.cc
 *
 * Make a new midi file.
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

#include "midi_make.h"

namespace vfx_ispeech
{
  /**
   * Make a new  midi file from some melody and rhythm.
   */
  midi_make::midi_make()
  {
  }

  /**
   *  Make a midi file.
   *
   * @Param midi file of midi
   * @Param melody input a lot of melody
   * @Param rhythm input a lot of rhythm
   * @Param tpq TicksPerQuarterNote
   */
  int midi_make::make(char* midi,vector<double> &melody,vector<double> &rhythm,double tpq)
  {
    if( (melody.size() <= 0) || (melody.size()!=rhythm.size()) )
      {
	return -1;
      }

    MidiFile outputfile;// create an empty MIDI file with one track
    outputfile.absoluteTicks();  // time information stored as absolute time
    // (will be coverted to delta time when written)
    outputfile.addTrack(1);     // Add another two tracks to the MIDI file
    vector<uchar> midievent;     // temporary storage for MIDI events
    midievent.resize(3);        // set the size of the array to 3 bytes
    //int tpq = 120;              // default value in MIDI file is 48
    outputfile.setTicksPerQuarterNote(tpq);

    // store a melody line in track 1 (track 0 left empty for conductor info)
    int actiontime = 0;      // temporary storage for MIDI event time
    midievent[2] = 64;       // store attack/release velocity for note command
    for(int i = 0; i < melody.size();i++) {
      midievent[0] = 0x90;     // store a note on command (MIDI channel 1)
      midievent[1] = melody[i];
      outputfile.addEvent(1, actiontime, midievent);
      actiontime += tpq * rhythm[i];
      midievent[0] = 0x80;     // store a note on command (MIDI channel 1)
      outputfile.addEvent(1, actiontime, midievent);
    }
    outputfile.sortTracks();// make sure data is in correct order
    outputfile.write(midi); // write Standard MIDI File twinkle.mid
    return 0;
  }
}

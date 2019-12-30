/**
 * @file midi_parser.cc
 *
 * Parser a midi file to melody sequence.
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

#include "midi_parser.h"
#include <list>

namespace vfx_ispeech
{
  /**
   * convert midi file to pitch array.
   */
  midi_parser::midi_parser()
  {
  }

  /**
   *  Parser the midi file.
   *
   * @Param midi file of midi
   * @Param pitch_array it save the all pitchs of parsed.
   */
  int midi_parser::parser(char* midi,float hop_size_in_ms,vector<MIDIDescriptorType> &pitch_array)
  {
    MidiFile midiFile;
    int       status;

    status = midiFile.read(midi);
    if (status == 0) {
      cout << "Syntax error in file: " << midi << "\n";
      return -1;
    }

    //midifile.absoluteTicks();

    int trackindex = 2;


    if(trackindex == 0)
      {
	midiFile.joinTracks();
      }

    //midiFile.mergeTracks(1,2);
    MidiEvent *event;
    NoteItem *currentNote = nullptr;
    std::list<NoteItem*> noteItemList;


    for (int eventID = 0;eventID < midiFile[trackindex].size(); eventID++) {

      double currentTime = midiFile.getTimeInSeconds(trackindex, eventID);
      event = &midiFile[trackindex][eventID];

      if(event->isNoteOn()) {

	// A note is started while previous note is not ended
	if(currentNote != nullptr) {
	  currentNote->end = currentTime;
	  noteItemList.push_back(currentNote);
	}

	currentNote = new NoteItem(currentTime, 0, event->getKeyNumber());
      }
      else if(event->isNoteOff()) {

	// There is no started note
	if(currentNote == nullptr) {
	  continue;
	}

	// If started note's pitch is different to event note's
	if((int)currentNote->note != event->getKeyNumber()) {
	  continue;
	}

	currentNote->end = currentTime;
	noteItemList.push_back(currentNote);
	currentNote = nullptr;
      }
    }

    for(std::list<NoteItem*>::iterator noteIterator = noteItemList.begin();
	noteIterator != noteItemList.end();
	noteIterator++) {
      NoteItem *currentNote = *noteIterator;
      int from_index = (int) floor(currentNote->start * 1000 / hop_size_in_ms);
      int to_index = (int) floor(currentNote->end * 1000 / hop_size_in_ms);
      from_index = std::max(from_index, 0);
      //to_index = std::min(to_index, pitchArrayLength);

      if(to_index >= from_index) {
	for(int index=from_index;index<to_index;index++)
	  {
	    MIDIDescriptorType midiDescriptor;
	    midiDescriptor.note = currentNote->note;
	    pitch_array.push_back(midiDescriptor);
	  }
      }

      delete currentNote;
    }

    return pitch_array.size();
  }

  vector<MIDIDescriptorType> midi_parser::parser2(char* midi,float hop_size_in_ms)
  {
    vector<MIDIDescriptorType> pitches;
    parser(midi,hop_size_in_ms,pitches);
    return pitches;
  }

}

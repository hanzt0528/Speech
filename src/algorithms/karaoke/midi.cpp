/**
 * @file Frame.cpp
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

#include "midi.h"

namespace vfx_ispeech
{
    /**
     * Creates the frame object - sets signal source and frame boundaries.
     *
     * Frame should not change original data, so the source is a const
     * reference.
     *
     * @param source const reference to signal source
     * @param indexBegin position of first sample of this frame in the source
     * @param indexEnd position of last sample of this frame in the source
     */
  midi::midi()
  {
  }

  int midi::take(int argc, char* argv[])
  {

    channelfilter.assign(16, 1);

    matlabarray.reserve(100000);
    matlabarray.clear();

    legend_instr.resize(128);
    legend_opcode.resize(128);
    legend_controller.resize(128);

    checkOptions(options, argc, argv);
    MidiFile midifile(options.getArg(1));
    convertMidiFile(midifile, matlabarray);
    if (!verboseQ) {
      //printMatlabArray(midifile, matlabarray);
      printNotesData(midifile, matlabarray);
    }
    return 0;
  }

  void midi::convertMidiFile(MidiFile& midifile, vector<vector<double> >& matlab) {
    midifile.absoluteTicks();
    midifile.joinTracks();
    vector<double> event(8);
    vector<double> ontimes(128);
    vector<int> onvelocities(128);
    int i;
    for (i=0; i<128; i++) {
      ontimes[i] = -1.0;
      onvelocities[i] = -1;
    }

    if (verboseQ) {
      cout << "-1\ttpq\t" << midifile.getTicksPerQuarterNote() << endl;
    }

    double offtime = 0.0;
    int key = 0;
    int vel = 0;

    for (i=0; i<midifile.getNumEvents(0); i++) {
      if (verboseQ) {
	cout << ">>> " << (int)midifile[0][i][0] << "\n";
      }
      event.assign(event.size(), unused);
      int command = midifile[0][i][0] & 0xf0;
      int channel = midifile[0][i][0] & 0x0f;

      // check for tempo indication
      if (midifile[0][i][0] == 0xff &&
	  midifile[0][i][1] == 0x51) {
	setTempo(midifile, i, tempo);
	if (verboseQ) {
	  cout << "# New Tempo: " << tempo << "\n";
	}
      }

      if ((midifile[0][i][0] & 0x0f) == 0x09) {
	continue;
      }
      if (command == 0xf0) {
	command = midifile[0][i][0];
      }
      if (command == 0x90 && midifile[0][i][2] != 0) {
	// store note-on velocity and time
	key = midifile[0][i][1];
	vel = midifile[0][i][2];
	ontimes[key] = getTime(midifile[0][i].tick, tempo,
			       midifile.getTicksPerQuarterNote());

	onvelocities[key] = vel;
      } else if (command == 0x90 || command == 0x80) {
	// note off command write to output
	key = midifile[0][i][1];
	offtime = getTime(midifile[0][i].tick, tempo,
			  midifile.getTicksPerQuarterNote());
	legend_opcode[OP_NOTE/1000] = 1;

	if (verboseQ) {
	  cout
	    << ontimes[key]
	    << "\tnote"
	    << "\tdur=" << offtime - ontimes[key]
	    << "\tpch=" << key
	    << "\tvel=" << onvelocities[key]
	    << "\tch="  << (midifile[0][i][0] & 0x0f)
	    << "\ttrack=" << midifile[0][i].track
	    << endl;
	} else {
	  event[0] = ontimes[key];
	  event[1] = OP_NOTE;
	  event[2] = offtime - ontimes[key];
	  event[3] = key;
	  event[4] = onvelocities[key];
	  event[5] = (midifile[0][i][0] & 0x0f);
	  event[6] = midifile[0][i].track;
	  event[7] = channel;
	}
      } else if (command == 0xb0) {
	legend_controller[midifile[0][i][1]] = 1;
	legend_opcode[OP_CONTROL/1000] = 1;

	if (verboseQ) {
	  cout << getTime(midifile[0][i].tick, tempo,
			  midifile.getTicksPerQuarterNote())
	       << "\tcontrol"
	       << "\ttype="  << (int)midifile[0][i][1]
	       << "\tval="   << (int)midifile[0][i][2]
	       << "\tch="    << (midifile[0][i][0] & 0x0f)
	       << "\ttrack=" << midifile[0][i].track
	       << "\n";
	} else {
	  event[0] = getTime(midifile[0][i].tick, tempo,
			     midifile.getTicksPerQuarterNote());
	  event[1] = OP_CONTROL;
	  event[2] = (int)midifile[0][i][1];
	  event[3] = (int)midifile[0][i][2];
	  event[5] = (midifile[0][i][0] & 0x0f);
	  event[6] = midifile[0][i].track;
	}
      } else if (command == 0xc0) {
	legend_instr[midifile[0][i][1]] = 1;
	legend_opcode[OP_INSTR/1000] = 1;

	if (verboseQ) {
	  cout << getTime(midifile[0][i].tick, tempo,
			  midifile.getTicksPerQuarterNote())
	       << "\tinstr"
	       << "\tname="  << GMinstrument[midifile[0][i][1]]
	       << "\tnum="   << (int)midifile[0][i][1]
	       << "\tch="    << (midifile[0][i][0] & 0x0f)
	       << "\ttrack=" << midifile[0][i].track
	       << "\n";
	} else {
	  event[0] = getTime(midifile[0][i].tick, tempo,
			     midifile.getTicksPerQuarterNote());
	  event[1] = OP_INSTR;
	  event[2] = (int)midifile[0][i][1];
	  event[5] = (midifile[0][i][0] & 0x0f);
	  event[6] = midifile[0][i].track;
	}
      } else if (command == 0xff) {
	if (verboseQ) {
	  cout << getTime(midifile[0][i].tick, tempo,
			  midifile.getTicksPerQuarterNote())
	       << "\t";
	} else {
	  event[0] = getTime(midifile[0][i].tick, tempo,
			     midifile.getTicksPerQuarterNote());
	}
	processMetaEvent(midifile, i, event);
	if (verboseQ) {
	  cout << "\n";
	}
      }

      if (event[1] != unused) {
	matlab.push_back(event);
      }
    }
  }



  //////////////////////////////
  //
  // processMetaEvent -- print the meta event
  //

  void midi::processMetaEvent(MidiFile& midifile, int i, vector<double>& event) {
    MidiEvent& data = midifile[0][i];

    switch (data[1]) {
    case 0x58:  // time signature
      // 58 04 nn dd cc bb
      //  nn=numerator of time sig.
      //  dd=denominator of time sig. 2=quarter
      //  3=eighth, etc.
      //  cc=number of ticks in metronome click
      //  bb=number of 32nd notes to the quarter note
      if (verboseQ) {
	cout << "meter\t" << (int)data[2] << "/" << pow(2.0, data[3]);
      } else {
	legend_opcode[OP_METER/1000] = 1;
	event[1] = OP_METER;
	event[2] = (int)data[2];
	event[3] = pow(2.0, data[3]);
      }
      break;
    case 0x59:  // key signature
      // 59 02 sf mi
      // sf=sharps/flats (-7=7 flats, 0=key of C, 7=7 sharps)
      // mi=major/minor (0=major, 1=minor)
      if (verboseQ) {
	cout << "keysig\t";
	if (data[3]==0) {
	  switch (data[2]) {
	  case 0: cout << "C-major"; break;
	  case 1: cout << "G-major"; break;
	  case 2: cout << "D-major"; break;
	  case 3: cout << "A-major"; break;
	  case 4: cout << "E-major"; break;
	  case 5: cout << "B-major"; break;
	  case 6: cout << "F-sharp-major"; break;
	  case 7: cout << "C-sharp-major"; break;
	  }
	} else {
	  switch (data[2]) {
	  case 0: cout << "A-minor"; break;
	  case 1: cout << "E-minor"; break;
	  case 2: cout << "B-minor"; break;
	  case 3: cout << "F-minor"; break;
	  case 4: cout << "C-sharp-minor"; break;
	  case 5: cout << "G-sharp-minor"; break;
	  case 6: cout << "D-sharp-minor"; break;
	  case 7: cout << "A-sharp-minor"; break;
	  }
	}
      } else {
	legend_opcode[OP_KEYSIG/1000] = 1;
	event[1] = OP_KEYSIG;
	event[2] = (int)data[2];
	event[3] = (int)data[3];
      }
      break;
    default:
      if (verboseQ) {
	cout << "meta\t0x" << hex << (int)data[1] << dec;
      }
    }
  }



  //////////////////////////////
  //
  // getTime -- return the time in command-line specified time unit
  //

  double midi::getTime(int ticks, double tempo, int tpq) {
    switch (timetype) {
    case TICK:
      return ticks;
    case BEAT:
      return (double)ticks/tpq;
    case SEC:
      return ticks*60.0/tempo/tpq;
    case MSEC:
      return ticks*60.0/tempo/tpq*1000.0;
    }
    return 0.0;
  }



  //////////////////////////////
  //
  // setTempo -- set the current tempo
  //

  void midi::setTempo(MidiFile& midifile, int index, double& tempo) {
    static int count = 0;
    count++;
    vector<double> event;
    event.assign(7, unused);

    MidiEvent& mididata = midifile[0][index];

    int microseconds = 0;
    microseconds = microseconds | (mididata[3] << 16);
    microseconds = microseconds | (mididata[4] << 8);
    microseconds = microseconds | (mididata[5] << 0);

    double newtempo = 60.0 / microseconds * 1000000.0;
    if (count <= 1) {
      tempo = newtempo;
    } else if (tempo != newtempo) {
      if (verboseQ) {
	cout << getTime(midifile[0][index].tick, tempo,
			midifile.getTicksPerQuarterNote())
	     << "\t"
	     << "tempo\t" << newtempo << endl;
      } else {
	legend_opcode[OP_TEMPO/1000] = 1;
	event[0] = getTime(midifile[0][index].tick, tempo,
			   midifile.getTicksPerQuarterNote());
	event[1] = OP_TEMPO;
	event[2] = newtempo;
	matlabarray.push_back(event);
      }
    }
    tempo = newtempo;
  }



  //////////////////////////////
  //
  // checkOptions --
  //

  void midi::checkOptions(Options& opts, int argc, char* argv[]) {
    opts.define("u|unused=d:-1000.0",              "unused parameter indicator");
    opts.define("n|name=s:data",                     "name for data array");
    opts.define("t|ticks|tick=b",                    "display time in ticks");
    opts.define("D|no-drum=b",                       "turn off drum track");
    opts.define("s|sec|second|seconds=b",            "display time in seconds");
    opts.define("m|msec|millisecond|milliseconds=b", "display time in msec");
    opts.define("b|beat|beats=b",                    "display time in beats");
    opts.define("x|exclude=s",                       "exclude channel notes");
    opts.define("v|verbose=b",                       "display verbose data");

    opts.define("author=b",  "author of program");
    opts.define("version=b", "compilation info");
    opts.define("example=b", "example usages");
    opts.define("h|help=b",  "short description");

    opts.define("debug=b",  "debug mode to find errors in input file");
    opts.define("max=i:100000", "maximum number of notes expected in input");

    opts.process(argc, argv);

    // handle basic options:
    if (opts.getBoolean("author")) {
      cout << "Written by Craig Stuart Sapp, "
	   << "craig@ccrma.stanford.edu, July 2007" << endl;
      exit(0);
    } else if (opts.getBoolean("version")) {
      cout << argv[0] << ", version: Aug 2007" << endl;
      cout << "compiled: " << __DATE__ << endl;
      exit(0);
    } else if (opts.getBoolean("help")) {
      usage(opts.getCommand().c_str());
      exit(0);
    } else if (opts.getBoolean("example")) {
      example();
      exit(0);
    }

    unused = opts.getDouble("unused");
    debugQ = opts.getBoolean("debug");
    maxcount = opts.getInteger("max");

    if (opts.getArgCount() != 1) {
      usage(opts.getCommand().c_str());
      exit(1);
    }

    setFilterOptions(channelfilter, opts.getString("exclude").c_str());
    if (opts.getBoolean("no-drum")) {
      // turn off MIDI channel 10
      channelfilter[9] = 0;
    }

    tickQ = opts.getBoolean("ticks");
    secQ  = opts.getBoolean("seconds");
    msecQ = opts.getBoolean("milliseconds");
    beatQ = opts.getBoolean("beats");
    strcpy(arrayname, opts.getString("name").c_str());

    if (tickQ) {
      timetype = TICK;
      if (verboseQ) {
	cout << "-1\ttunit\tticks\n";
      }
    } else if (beatQ) {
      timetype = BEAT;
      if (verboseQ) {
	cout << "-1\ttunit\tbeat\n";
      }
    } else if (secQ) {
      timetype = SEC;
      if (verboseQ) {
	cout << "-1\ttunit\tseconds\n";
      }
    } else if (msecQ) {
      timetype = MSEC;
      if (verboseQ) {
	cout << "-1\ttunit\tmilliseconds\n";
      }
    } else {
      timetype = BEAT;
      if (verboseQ) {
	cout << "-1\ttunit\tseconds\n";
      }
    }
  }



  //////////////////////////////
  //
  // setFilterOptions --
  //

  void midi::setFilterOptions(vector<int>& channelfilter, const char* exclude) {
    int length = (int)strlen(exclude);
    int value;

    for (int i=0; i<length; i++) {
      int character = toupper(exclude[i]);
      if (!isxdigit(character)) {
	continue;
      }
      if (isdigit(character)) {
	value = character - '0';
      } else {
	value = character - 'A' + 10;
      }
      if (value >= 0 && value <= 15) {
	channelfilter[value] = 0;
      }
    }
  }



  //////////////////////////////
  //
  // example --
  //

  void midi::example(void) {

  }



  //////////////////////////////
  //
  // usage --
  //

  void midi::usage(const char* command) {
    cout << "Usage: " << command << " midifile" << endl;
  }



  //////////////////////////////
  //
  // printLegend -- print a legend of the codes in the data.
  //


  void midi::printLegend(MidiFile& midifile) {
    int sum = 0;
    int i;

    cout << "\n";
    cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n";
    cout << "%% DATA LEGEND                                               %%\n";
    cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n";
    switch (timetype) {
    case TICK: cout << "% time unit: ticks\n";         break;
    case BEAT: cout << "% time unit: beat\n";          break;
    case SEC:  cout << "% time unit: seconds\n";       break;
    case MSEC: cout << "% time unit: milliseconds\n";  break;
    default:   cout << "% time unit: unknown\n";

    }
    cout << "% Ticks per quarter note: " << midifile.getTicksPerQuarterNote()
	 << "\n";
    cout << "% unused parameter marker: " << unused << "\n";
    // check for opcodes used in data:
    sum = 0;
    for (i=0; i<(int)legend_opcode.size(); i++) {
      sum += legend_opcode[i];
    }
    if (sum > 0) {
      cout << "% " << sum << " opcodes are present in the data:\n";
      for (i=0; i<(int)legend_opcode.size(); i++) {
	if (legend_opcode[i]) {
	  switch (i*1000) {
	  case OP_NOTE:
	    cout << "%\topcode " << OP_NOTE << "\t= note\n";
	    cout << "%\t\tp1 = start time of note\n";
	    cout << "%\t\tp2 = opcode for note\n";
	    cout << "%\t\tp3 = duration of note\n";
	    cout << "%\t\tp4 = MIDI key number\n";
	    cout << "%\t\tp5 = MIDI attack velocity\n";
	    cout << "%\t\tp6 = MIDI channel\n";
	    cout << "%\t\tp7 = MIDI-file track number\n";
	    break;
	  case OP_TEMPO:
	    cout << "%\topcode " << OP_TEMPO << "\t= tempo change\n";
	    cout << "%\t\tp1 = start time of tempo\n";
	    cout << "%\t\tp2 = opcode for tempo\n";
	    cout << "%\t\tp3 = number of beats per minute\n";
	    cout << "%\t\tp4-p7 = unused\n";
	    break;
	  case OP_CONTROL:
	    cout << "%\topcode " << OP_CONTROL
		 << "\t= continuous controller\n";
	    cout << "%\t\tp1 = action time of controller\n";
	    cout << "%\t\tp2 = opcode for controller\n";
	    cout << "%\t\tp3 = controller number\n";
	    cout << "%\t\tp4 = controller value\n";
	    cout << "%\t\tp5 = unused\n";
	    cout << "%\t\tp6 = MIDI channel\n";
	    cout << "%\t\tp7 = MIDI-file track number\n";
	    break;
	  case OP_INSTR:
	    cout << "%\topcode " << OP_INSTR << "\t= instrument\n";
	    cout << "%\t\tp1 = start time of instrument on channel\n";
	    cout << "%\t\tp2 = opcode for instrument\n";
	    cout << "%\t\tp3 = instrument number\n";
	    cout << "%\t\tp4-p5 = unused\n";
	    cout << "%\t\tp6 = MIDI channel\n";
	    cout << "%\t\tp7 = MIDI-file track number\n";
	    break;
	  case OP_METER:
	    cout << "%\topcode " << OP_METER << "\t= meter signature\n";
	    cout << "%\t\tp1 = start time of meter signature\n";
	    cout << "%\t\tp2 = opcode for meter\n";
	    cout << "%\t\tp3 = numerator of meter\n";
	    cout << "%\t\tp4 = denominator of meter\n";
	    cout << "%\t\tp5-p7 = unused\n";
	    break;
	  case OP_KEYSIG:
	    cout << "%\topcode " << OP_KEYSIG << "\t= key signature\n";
	    cout << "%\t\tp1 = start time of key signature\n";
	    cout << "%\t\tp2 = opcode for key signature\n";
	    cout << "%\t\tp3 = number of sharps (positive) or flats (negative)\n";
	    cout << "%\t\tp4 = mode (0=major, 1=minor)\n";
	    cout << "%\t\tp5-p7 = unused\n";
	    break;
	  default:
	    cout << "%\topcode " << i*1000 << "\t= unknown\n";
	  }
	}
      }
    }


    // check for instruments to list:
    sum = 0;
    for (i=0; i<(int)legend_instr.size(); i++) {
      sum += legend_instr[i];
    }
    if (sum > 0) {
      cout << "% " << sum << " instrument timbres are present in the data:\n";
      for (i=0; i<(int)legend_instr.size(); i++) {
	if (legend_instr[i]) {
	  cout << "%\tinstrument number " << i << "\t= "
	       << GMinstrument[i] << "\n";
	}
      }
    }

    // check for controllers to list:
    sum = 0;
    for (i=0; i<(int)legend_controller.size(); i++) {
      sum += legend_controller[i];
    }
    if (sum > 0) {
      cout << "% " << sum << " types of controllers are present in the data:\n";
      for (i=0; i<(int)legend_controller.size(); i++) {
	if (legend_controller[i]) {
	  cout << "%\tcontroller " << GMcontrollers[i] << "\n";
	}
      }
    }

    cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n";

  }



  //////////////////////////////
  //
  // printMatlabArray -- print the Matlab array representing the MIDI file.
  //

  void midi::printMatlabArray(MidiFile& midifile, vector<vector<double> >& matlab) {
    int i;
    sortArray(matlab);
    cout << arrayname << " = [\n";
    for (i=0; i<(int)matlab.size(); i++) {
      printEvent(matlab[i]);
    }
    cout << "];\n";
    printLegend(midifile);
  }

  //////////////////////////////
  //
  // printNotesData  -- print the Matlab array representing the MIDI file.
  //

  void midi::printNotesData(MidiFile& midifile, vector<vector<double> >& matlab) {
    int i;
    sortArray(matlab);
    // cout << arrayname << " = [\n";
    for (i=0; i<(int)matlab.size(); i++) {
      printNotesEvent(matlab[i]);
    }
    // cout << "];\n";
    // printLegend(midifile);
  }



  //////////////////////////////
  //
  // eventcmp -- compare two events and determine which should come
  //    first in time.
  //

  int eventcmp(const void* a, const void* b) {
    vector<double>& A = *((vector<double>*)a);
    vector<double>& B = *((vector<double>*)b);

    if (A[0] < B[0]) {
      return -1;
    } else if (A[0] > B[0]) {
      return 1;
    } else {
      return 0;   // there is a tie if they come at the same time.
    }
  }

  //////////////////////////////
  //
  // sortArray -- sort the input file into time order because
  //   the notes may be slightly out of time order.
  //

  void midi::sortArray(vector<vector<double> >& matlab) {
    qsort(matlab.data(), matlab.size(), sizeof(vector<double>), eventcmp);
  }





  //////////////////////////////
  //
  // printEvent -- print the event
  //

  void midi::printEvent(vector<double>& event) {
    int i;
    for (i=0; i<(int)event.size(); i++) {
      cout << event[i];
      if (i<(int)event.size()-1) {
	cout << ",\t";
      }
    }
    cout << ";\n";
  }



  //////////////////////////////
  //
  // printNotesEvent -- print the event
  //            event[0] = ontimes[key];
  //            event[1] = OP_NOTE;
  //            event[2] = offtime - ontimes[key];
  //            event[3] = key;
  //            event[4] = onvelocities[key];
  //            event[5] = (midifile[0][i][0] & 0x0f);
  //            event[6] = midifile[0][i].track;

  void midi::printNotesEvent(vector<double>& event) {
    if (int(event[1]+0.5) != OP_NOTE) {
      // ignore items which are not notes.
      return;
    }
    int ontime = int(event[0] * 1000 + 0.5);
    if (ontime < 0) {
      // ignore incomplete note events (no on times)
      return;
    }
    if (channelfilter[int(event[7]+0.5)] == 0) {
      // ignore excluded channel notes (for drum track exclusions)
      return;
    }
    cout << "Note\t";
    cout << ontime; // ontime in milliseconds
    cout << '\t';
    cout << int((event[0]+event[2]) * 1000 + 0.5); // endtime in milliseconds
    cout << '\t';
    cout << int(event[3] + 0.5); // Pitch (MIDI note);
    if (debugQ) {
      cout << "\tCH_" << (int)(event[7]+ 0.5) + 1;
    }
    cout << endl;

    /*   int i;
	 for (i=0; i<(int)event.size(); i++) {
	 cout << event[i];
	 if (i<(int)event.size()-1) {
	 cout << ",\t";
	 }
	 }
	 cout << ";\n";
    */
  }


}

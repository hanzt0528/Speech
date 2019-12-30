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
#include "karaoke.h"

#include "ispeech/ispeech.h"
#include "./midi_parser.h"

namespace vfx_ispeech
{
  /**
   * Make a new  midi file from some melody and rhythm.
   */
  karaoke::karaoke()
  {
  }

  /**
   *  Setup some info.
   *
   * @Param midi file of midi
   * @Param wav file of wav,need 44100hz,mono track.
   * @Param hopsize_ms frame last time
   */
  int karaoke::set(std::string midi,std::string wav,double hopsize_ms,double threshold/*=23.0*/)
  {
    WaveFile s(wav);
    LOG_WRN(nullptr,"Origin Signal Samples Count=%d",s.size());

    uint64_t fs = s.getBytesPerSec()/s.getBytesPerSample();

    LOG_WRN(nullptr,"fs = %lld",fs);

    double frame_size_in_ms = hopsize_ms;//ms

    FramesCollection frames(s,frame_size_in_ms/1000*fs);
    LOG_WRN(nullptr,"frame count = %lld",frames.count());

    float divisor = float(CLOCKS_PER_SEC) / 1000.f;
    clock_t start = clock();

    int i = 0;
    SampleListType sound_pitches;

    wav_.clear();

    double last = 0;
    for (auto it = frames.begin(); it != frames.end(); ++it,i++)
      {
	double pitch = 0.0;
	uint64_t pos = 0;

	//LOG_WRN(nullptr,"it->size = %lld",it->size());
	PitchDetection pitchDetection(it->size());

	/*
	HammingWindow hamming = HammingWindow(it->size());
	SignalSource s(it->toArray(),it->size());
	s *= hamming;
	*/

	double pitch_r = pitchDetection.Detect(it->toArray(),44100.0,pitch,pos);
	double mesi= 69+12*log2(pitch/440.0);

	SoundDescriptorType sd;
	sd.pitch = mesi;

	wav_.push_back(sd);

	/*
	if(i > 3)
	  {
	    if(abs(wav_[i]-wav_[i-1]) > 10)
	      {
		if( (wav_[i]-wav_[i-2]) < 5)
		  {
		    //wav_[i-1] = wav_[i-2];
		  }
	      }
	  }

	if(i > 5)
	  {
	    for(int k = 0; k < 5; k++)
	      {
		//wav_[i-k]= (wav_[i]+wav_[i-1]+wav_[i-2]+wav_[i-3]+wav_[i-4])/5;
	      }
	  }
	*/
	//LOG_WRN(nullptr,"pitch of frame[%d] = %f,mesi = %f,pos = %lld",i,pitch,mesi,pos);
      }

    clock_t end = clock();

    LOG_WRN(nullptr,"Time for wav to melody method  = %f",float(end - start)/divisor);

      //midiParser
     midi_parser midiParser;
     std::vector<double> pitchs;

     //char * midi = "../data/twinkle.mid";
     //char * midi = "../data/YouAndi.mid";
     //char * midi = "../data/midi/HappyBirthday.mid";
     //char * midi = "../data/midi/national anthems/China.mid";

     midi_.clear();
     int nCount = midiParser.parser((char*)midi.c_str(),frame_size_in_ms,midi_);

     LOG_WRN(nullptr,"test_midi_parser: count = %d",nCount);

     for(std::size_t k = 0; k < midi_.size();k++)
       {
	 //LOG_WRN(nullptr,"test_midi_parser: pitchs[%lld] = %f",k,pitchs[k]);
       }

     std::size_t pitch_count = 0;
     wav_.size() > midi_.size()?pitch_count=midi_.size():pitch_count = wav_.size();

     double score = 0.0;
     int offset = 0;//sound_pitches.size()-pitch_count;



     //double newbest = filter.test(std::vector<double>(wav_.begin(),wav_.begin()+pitch_count));


     for(std::size_t i = 0 ; i < pitch_count; i++)
       {
	 double diff =std::abs(wav_[i+offset].pitch - midi_[i].note);

	 LOG_WRN(nullptr,"sound = %f,midi=%f,diff = %f",wav_[i+offset].pitch,midi_[i].note,diff);

	 if(  diff<= threshold)
	   {
	     score+=1.0;
	   }
       }
     /*
     int window_size = 10;

     int pin = 0;

     while(pin < (pitch_count-window_size) )
       {

	 std::vector<double> midi(midi_.begin()+pin,midi_.begin()+pin+window_size);

	 LB_Improved filter(midi, 2); // we use the DTW with a tolerance of 10% (size/10)

	 std::vector<double> notes(wav_.begin()+pin,wav_.begin()+pin+window_size);
	 double newbest = filter.test(notes);
	 LOG_WRN(nullptr,"******DTW newbest = %d",newbest);

	 pin+=window_size;
       }
     */
     score_ = score/pitch_count*100;
    return 1;
  }
  std::vector<MIDIDescriptorType> karaoke::getmidi()
  {
    return midi_;
  }
  std::vector<SoundDescriptorType> karaoke::getwav()
  {
    return wav_;
  }

  double karaoke::getscore()
  {
    return score_;
  }

}

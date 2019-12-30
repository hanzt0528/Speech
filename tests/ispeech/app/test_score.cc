#include "ispeech/ispeech.h"

#include <time.h>
#include "../../../src/algorithms/karaoke/midi_parser.h"
using namespace vfx_ispeech;
int main(int argc,char** argv)
{
  LOG_WRN(nullptr,"pitchdetection");

  /*
  SineGenerator s(44100);
  s.setFrequency(1206);
  s.setAmplitude(1.0);
  s.setPhase(0.1);
  s.generate(4000);
  */

  //twinkle.mid
  //int melody[50]  = {72,72,79,79,81,81,79,77,77,76,76,74,74,72,-1};
  //int mrhythm[50] = { 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2,-1};
  //WaveFile s("../data/twinkle.wav");
  //WaveFile s("../data/twinkle_beiwa.wav");
  //WaveFile s("../data/YouAndI_midi_mono.wav");
  // WaveFile s("../data/twinkle_me.wav");
  //WaveFile s("../data/mix.wav");
  //WaveFile s("../data/A4-440Hz-69.wav");
  //WaveFile s("../data/sine-1000.wav");
  
  //WaveFile s("../data/midi/HappyBirthday.wav");
  //WaveFile s("../data/midi/HappyBirthday_gband_make.wav");

  WaveFile s("../data/midi/national anthems/China.wav");
  LOG_WRN(nullptr,"Origin Signal Samples Count=%d",s.size());

  LOG_WRN(nullptr,"\nOrigin:");


  uint64_t fs = s.getBytesPerSec()/s.getBytesPerSample();

  LOG_WRN(nullptr,"fs = %lld",fs);

  double frame_size_in_ms = 80;//ms

  FramesCollection frames(s,frame_size_in_ms/1000*fs);
  LOG_WRN(nullptr,"frame count = %lld",frames.count());

  float divisor = float(CLOCKS_PER_SEC) / 1000.f;

  clock_t start = clock();
    
  int i = 0;
  SampleListType sound_pitches;
  for (auto it = frames.begin(); it != frames.end(); ++it,i++)
    {
      double pitch = 0.0;
      uint64_t pos = 0;

      //LOG_WRN(nullptr,"it->size = %lld",it->size());
      PitchDetection pitchDetection(it->size());

      double pitch_r = pitchDetection.Detect(it->toArray(),44100.0,pitch,pos);
      double mesi= 69+12*log2(pitch/440.0);
      sound_pitches.push_back(mesi);
      //LOG_WRN(nullptr,"pitch of frame[%d] = %f,mesi = %f,pos = %lld",i,pitch,mesi,pos);
    }

     clock_t end = clock();

     LOG_WRN(nullptr,"Time for naive method  = %f",float(end - start)/divisor);

     //midiParser
     midi_parser midiParser;
     std::vector<MIDIDescriptorType> pitchs;

     //char * midi = "../data/twinkle.mid";
     //char * midi = "../data/YouAndi.mid";
     //char * midi = "../data/midi/HappyBirthday.mid";
     char * midi = "../data/midi/national anthems/China.mid";

     int nCount = midiParser.parser(midi,frame_size_in_ms,pitchs);
  
     LOG_WRN(nullptr,"test_midi_parser: count = %d",nCount);

     for(std::size_t k = 0; k < pitchs.size();k++)
       {
	 //LOG_WRN(nullptr,"test_midi_parser: pitchs[%lld] = %f",k,pitchs[k]);
       }

     std::size_t pitch_count = 0;
     sound_pitches.size() > pitchs.size()?pitch_count=pitchs.size():pitch_count = sound_pitches.size();

     double score = 0.0;
     int offset = 0;//sound_pitches.size()-pitch_count;

     
     for(std::size_t i = 0 ; i < pitch_count; i++)
       {
	 double diff =std::abs(sound_pitches[i+offset] - pitchs[i].note);
	 
	 LOG_WRN(nullptr,"sound = %f,midi=%f,diff = %f",sound_pitches[i+offset],pitchs[i].note,diff);    

	 if(  diff<= 23)
	   {
	     score+=1.0;
	   }
       }

     LOG_WRN(nullptr,"finally score = %f,followed=%f,total=%lld",score/pitch_count*100,score,pitch_count);    
  
  return 0;
}

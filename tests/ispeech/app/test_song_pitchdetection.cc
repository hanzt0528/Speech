#include "ispeech/ispeech.h"

#include <time.h>

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
  WaveFile s("../data/twinkle.wav");
  //WaveFile s("../data/A4-440Hz-69.wav");
  //WaveFile s("../data/sine-1000.wav");
  LOG_WRN(nullptr,"Origin Signal Samples Count=%d",s.size());

  LOG_WRN(nullptr,"\nOrigin:");


  uint64_t fs = s.getBytesPerSec()/s.getBytesPerSample();

  LOG_WRN(nullptr,"fs = %lld",fs);

  FramesCollection frames(s,1*fs);
  LOG_WRN(nullptr,"frame count = %lld",frames.count());

  float divisor = float(CLOCKS_PER_SEC) / 1000.f;

  clock_t start = clock();
    
  int i = 0; 
  for (auto it = frames.begin(); it != frames.end(); ++it,i++)
    {
      double pitch = 0.0;
      uint64_t pos = 0;

      //LOG_WRN(nullptr,"it->size = %lld",it->size());
      PitchDetection pitchDetection(it->size());

      double pitch_r = pitchDetection.Detect(it->toArray(),44100.0,pitch,pos);
      double mesi= 69+12*log2(pitch/440.0);
      LOG_WRN(nullptr,"pitch of frame[%d] = %f,mesi = %f,pos = %lld",i,pitch,mesi,pos);
    }

     clock_t end = clock();

     LOG_WRN(nullptr,"Time for naive method  = %f",float(end - start)/divisor);
  
    
  return 0;
}

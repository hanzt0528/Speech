#include "ispeech/ispeech.h"

using namespace vfx_ispeech;
int main(int argc,char** argv)
{
  SineGenerator s(44100);
  s.setFrequency(880);
  s.setAmplitude(2.0);
  s.setPhase(0.1);
  s.generate(5*44100);

  SineGenerator s2(44100);
  s2.setFrequency(440);
  s2.setAmplitude(2.0);
  s2.setPhase(0.1);
  s2.generate(5*44100);

  s +=s2;
  
  LOG_WRN(nullptr,"Origin Signal Samples Count=%d",s.size());
  LOG_WRN(nullptr,"\nOrigin:");
  
  int i = 0;

  s*=1024;
  
  for (auto it = s.begin(); it != s.end(); it++, i++)
    {
      LOG_WRN(nullptr,"Sample[%d]=%f",i,s.sample(i));
    }

  WaveFile::save(s,"sine_440_and_880_hz.wav");
  
  return 0;
}

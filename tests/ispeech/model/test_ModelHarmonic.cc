#include "ispeech/ispeech.h"

using namespace vfx_ispeech;
int main(int argc,char** argv)
{

  LOG_WRN(nullptr,"test ModelHarmonic");

  std::size_t M = 256;
  std::size_t N = 512;
  
  SineGenerator s(44100);
  s.setFrequency(880);
  s.setAmplitude(1.0);
  s.setPhase(0.1);
  s.generate(M);

  HammingWindow w(M);

  ModelHarmonic model;

  SampleListType f0 = model.f0Detection(s,44100,w,N,64,-100,400,1000,1);

  LOG_WRN(nullptr,"ModelHarmonic f0Detection f0.size = %lld\n",f0.size());
  for(std::size_t i = 0 ; i < f0.size(); i++)
    {
      LOG_WRN(nullptr,"ModelHarmonic f0Detection f0[%lld] = %f\n",i,f0[i]);      
    }
  return 0;
}

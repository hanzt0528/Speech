#include "ispeech/ispeech.h"

using namespace vfx_ispeech;
int main(int argc,char** argv)
{

  LOG_WRN(nullptr,"test ModelDFT");

  std::size_t M = 256;
  std::size_t N = 512;
  
  SineGenerator s(44100);
  s.setFrequency(880);
  s.setAmplitude(1.0);
  s.setPhase(0.1);
  s.generate(M);

  HammingWindow w(M);

  ModelDFT model;

  SampleListType y = model.dftModel(s,w,N);

  std::size_t size_y = y.size();
  
  for (std::size_t i = 0; i < size_y; i++)
    {
      LOG_WRN(nullptr,"y[%lld] = %f",i,y[i]);
    }

  return 0;
}

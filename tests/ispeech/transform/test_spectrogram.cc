#include "ispeech/ispeech.h"

using namespace vfx_ispeech;
int main(int argc,char** argv)
{
  LOG_WRN(nullptr,"spectrogram");
  
  SineGenerator s(44100);
  s.setFrequency(880);
  s.setAmplitude(1.0);
  s.setPhase(0.1);
  s.generate(44100);

  FramesCollection frames(s,256);
  Spectrogram sgm(frames);
  LOG_WRN(nullptr,"getFrameCount  = %d",sgm.getFrameCount());
  LOG_WRN(nullptr,"getSpectrumSize  = %d",sgm.getSpectrumSize());

  double sum = 0;
  for(int i = 0; i < sgm.getSpectrumSize()/2; i++)
    {
      ComplexType c = sgm.getPoint(1,i);
      //LOG_WRN(nullptr,"Point[%d]  = %f dB",i,dB(std::abs(c)));
      LOG_WRN(nullptr,"Point[%d]  = %f ",i,std::abs(c));
      sum+=std::abs(c);            
    }
  LOG_WRN(nullptr,"sum  = %f ",sum);

  return 0;
}

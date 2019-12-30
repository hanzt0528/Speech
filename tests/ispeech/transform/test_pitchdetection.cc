#include "ispeech/ispeech.h"

using namespace vfx_ispeech;
int main(int argc,char** argv)
{
  LOG_WRN(nullptr,"pitchdetection");
  
  SineGenerator s(44100);
  s.setFrequency(1206);
  s.setAmplitude(1.0);
  s.setPhase(0.1);
  s.generate(4000);

  double pitch = 0.0;
  uint64_t pos = 0;
  PitchDetection pitchDetection(4000);

  double pitch_r = pitchDetection.Detect(s.toArray(),44100.0,pitch,pos);
  LOG_WRN(nullptr,"pitch of signal = %f,pos = %lld",pitch,pos);

  return 0;
}

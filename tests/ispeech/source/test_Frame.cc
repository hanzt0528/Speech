#include "ispeech/ispeech.h"

using namespace vfx_ispeech;
int main(int argc,char** argv)
{


  WaveFile s("../data/16b_mono.wav");
  
  LOG_WRN(nullptr,"Origin wav Samples Count=%d",s.size());


  Frame f(s,0,100);

  LOG_WRN(nullptr,"a Frame Samples Count=%d",f.size());
  
  int i = 0;
  for (auto it = f.begin(); it != f.end(); it++, i++)
    {
      LOG_WRN(nullptr,"Frame Sample[%d]=%f",i,f.sample(i));
    }
  return 0;
}

#include "ispeech/ispeech.h"

using namespace vfx_ispeech;
int main(int argc,char** argv)
{


  WaveFile s("../data/soprano-E4.wav");

  LOG_WRN(nullptr,"Origin Signal Samples Count=%d",s.size());

  LOG_WRN(nullptr,"\nOrigin:");

  double *pa = new double[15];

  LPCBurg(s.toArray(),512,pa,15);

  int i = 0;
  for (auto it = s.begin(); it != s.end(); it++, i++)
    {
      // LOG_WRN(nullptr,"Sample[%d]=%f",i,s.sample(i));
    }

  for(int i = 0; i < 15; i++)
    {
      LOG_WRN(nullptr,"coefficient[%d]=%f",i,pa[i]);
    }
  /*
    LOG_WRN(nullptr,"\ns+=2.0:");
    s+=2.0;
    i = 0;
    for (auto it = s.begin(); it != s.end(); it++, i++)
    {
    LOG_WRN(nullptr,"Sample[%d]=%f",i,s.sample(i));
    }

    LOG_WRN(nullptr,"\ns*=2.0:");
    s*=2.0;
    i = 0;
    for (auto it = s.begin(); it != s.end(); it++, i++)
    {
    LOG_WRN(nullptr,"Sample[%d]=%f",i,s.sample(i));
    }
  */
  return 0;
}

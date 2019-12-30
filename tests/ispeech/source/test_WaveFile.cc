#include "ispeech/ispeech.h"

using namespace vfx_ispeech;
int main(int argc,char** argv)
{


  WaveFile s("../data/16b_mono.wav");
  
  LOG_WRN(nullptr,"Origin Signal Samples Count=%d",s.size());

  LOG_WRN(nullptr,"\nOrigin:");

  s*=2.0;


  WaveFile::save(s,"t.wav");
  
  int i = 0;
  for (auto it = s.begin(); it != s.end(); it++, i++)
    {
      LOG_WRN(nullptr,"Sample[%d]=%f",i,s.sample(i));
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

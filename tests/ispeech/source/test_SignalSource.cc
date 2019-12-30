#include "ispeech/ispeech.h"

using namespace vfx_ispeech;
int main(int argc,char** argv)
{

  SignalSource s(std::vector<SampleType>{1.0,2.0,3.0,4.0,5.0});

  LOG_WRN(nullptr,"Origin Signal Samples Count=%d",s.size());

  LOG_WRN(nullptr,"\nOrigin:");

  int i = 0;
  for (auto it = s.begin(); it != s.end(); it++, i++)
    {
      LOG_WRN(nullptr,"Sample[%d]=%f",i,s.sample(i));
    }
  
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
  
  return 0;
}

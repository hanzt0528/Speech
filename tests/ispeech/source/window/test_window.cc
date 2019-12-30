#include "ispeech/ispeech.h"

using namespace vfx_ispeech;
int main(int argc,char** argv)
{


  HannWindow s(20);  
  LOG_WRN(nullptr,"Origin Signal Samples Count=%d",s.size());

  LOG_WRN(nullptr,"\nOrigin:");
  
  int i = 0;
  for (auto it = s.begin(); it != s.end(); it++, i++)
    {
      LOG_WRN(nullptr,"Sample[%d]=%f",i,s.sample(i));
    }
  return 0;
}

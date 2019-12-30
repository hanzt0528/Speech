#include "ispeech/ispeech.h"

using namespace vfx_ispeech;
int main(int argc,char** argv)
{
  
  LOG_WRN(nullptr,"dB(100.0) = %f",dB(100.0,10.0));
  LOG_WRN(nullptr,"clamp = %f",clamp(10.2,10.1,20.0));
  LOG_WRN(nullptr,"randomDouble = %f",randomDouble());
  LOG_WRN(nullptr,"randomDouble = %f",randomDouble());
  LOG_WRN(nullptr,"1024 is power of 2:%d",isPowerOf2(1024));
  LOG_WRN(nullptr,"2048 is power of 2:%d",isPowerOf2(2048));
  LOG_WRN(nullptr,"1023 is power of 2:%d",isPowerOf2(1023));
  LOG_WRN(nullptr,"1023 next power of 2 is :%d",nextPowerOf2(1023));
  LOG_WRN(nullptr,"1025 next power of 2 is :%d",nextPowerOf2(1025));
  LOG_WRN(nullptr,"euclideanDistance of <0.0,0.0> and <3.0,4.0> is :%f",euclideanDistance(std::vector<double>{0.0,0.0},std::vector<double>{3.0,4.0}));

  //test cleaningSineTracks
  std::vector<SampleListType> xtfreq;
  for(std::size_t f = 0; f < 10; f++)
    {
      SampleListType tfreq(10,0);
      for(std::size_t t = 0; t < 3; t++)
	{
	  if(t == 0 && (f < 9))
	    {
	      tfreq[t]= 300;
	    }

	  if(t == 1 && (f >= 0 && f < 4))
	    {
	      tfreq[t]= 400;
	    }
	}
      xtfreq.push_back(tfreq);
    }
  
  LOG_WRN(nullptr,"orignal tracks:");
  for(std::size_t t = 0; t < 3; t++)
    {
      LOG_WRN(nullptr,"-----track[%lld]-----",t);
      for(std::size_t f = 0; f < 10; f++)
	{
	  LOG_WRN(nullptr,"track[%lld] frame[%lld] = %f",t,f,xtfreq[f][t]);
	}
    }

  cleaningSineTracks(xtfreq,4);
  LOG_WRN(nullptr,"cleaningsinetracks tracks:");
  for(std::size_t t = 0; t < 3; t++)
    {
      LOG_WRN(nullptr,"-----track[%lld]-----",t);
      for(std::size_t f = 0; f < 10; f++)
	{
	  LOG_WRN(nullptr,"track[%lld] frame[%lld] = %f",t,f,xtfreq[f][t]);
	}
    }

  //test SampleListType allocate
  SampleListType s1(10,11);
  for(std::size_t k = 0; k < 10; k++)
    {
      s1[k] = k;
    }

  SampleListType s2(s1.begin(),s1.end());
  for(std::size_t k = 0; k < 10; k++)
    {
      LOG_WRN(nullptr,"s2[%lld] = %f",s2[k]);
   }
  
  return 0;
}

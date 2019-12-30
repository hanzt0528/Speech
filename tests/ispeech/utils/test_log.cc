#include "ispeech/ispeech.h"

using namespace vfx_ispeech;
int main(int argc,char** argv)
{
  
  LOG_ERR(nullptr,"test error");
  LOG_INF(nullptr,"test info");
  LOG_MSG(nullptr,"test message");
  LOG_WRN(nullptr,"test warning");
  LOG_ERR(nullptr,"error:cout = %d",12);
  LOG_ERR(nullptr,"error:cout = %.2f",12.123456);
  LOG_ERR(nullptr,"error:cout = %f",12.123456);
  LOG_WRN(nullptr,"error:cout = %lld",123456789);
  
  return 0;
}

#include "ispeech/ispeech.h"

using namespace vfx_ispeech;
int main(int argc,char** argv)
{
  ComplexType c(2,3);
  LOG_WRN(nullptr,"real of complex(2+3i) = %f",c.real());
  LOG_WRN(nullptr,"imag of complex(2+3i) = %f",c.imag());

  return 0;
}

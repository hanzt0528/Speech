#include "ispeech/ispeech.h"
#include "../../../src/algorithms/karaoke/midi.h"
using namespace vfx_ispeech;

int main(int argc,char** argv)
{
  midi md;
  LOG_WRN(nullptr,"test_midi:");

  md.take(argc,argv);
  

  return 0;
}

#include "ispeech/ispeech.h"
#include "../../../src/algorithms/karaoke/midi_make.h"
using namespace vfx_ispeech;

int main(int argc,char** argv)
{
  LOG_WRN(nullptr,"test_midi_make:");

  midi_make midiMaker;
  std::vector<double> melody{72,72,79,79,81,81,79,77,77,76,76,74,74,72};
  std::vector<double> rhythm{ 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2};

  int hr = midiMaker.make("../data/twinkle.mid",melody,rhythm,250);

  if(hr < 0)
    {
      LOG_ERR(nullptr,"test_midi_make fialed!");      
    }
  return 0;
}

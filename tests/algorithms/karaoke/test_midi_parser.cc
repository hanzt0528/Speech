#include "ispeech/ispeech.h"
#include "../../../src/algorithms/karaoke/midi_parser.h"
using namespace vfx_ispeech;

int main(int argc,char** argv)
{
  midi_parser midiParser;
  std::vector<MIDIDescriptorType> pitchs;

  int nCount = midiParser.parser("../data/twinkle.mid",1000,pitchs);
  
  LOG_WRN(nullptr,"test_midi_parser: count = %d",nCount);

  for(std::size_t k = 0; k < pitchs.size();k++)
    {
      LOG_WRN(nullptr,"test_midi_parser: pitchs[%lld] = %f",k,pitchs[k].note);
    }

  return 0;
}

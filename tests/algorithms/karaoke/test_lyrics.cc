#include "ispeech/ispeech.h"
#include "../../../src/algorithms/karaoke/lyrics.h"
using namespace vfx_ispeech;

int main(int argc,char** argv)
{
  LOG_WRN(nullptr,"test_lyrics:");

  readLyric("../data/midi/axi.lrc");
  

  return 0;
}

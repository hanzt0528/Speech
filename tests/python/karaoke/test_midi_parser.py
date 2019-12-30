import pdb
import numpy as np 
import matplotlib.pyplot as plt
from ispeech.source import *
from ispeech.transform import *

def test():
  

    mparser = midi_parser();

    pitches = mparser.parser2('../../../data/midi/national anthems/China.mid',5);

    
    plt.subplot(2,1,1)
    pitches = np.array(pitches);

    plt.plot(np.arange(pitches.size),pitches,'-');

    plt.legend()
    plt.show()

test()

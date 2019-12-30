import pdb
import numpy as np 
import matplotlib.pyplot as plt
from ispeech.source import *
from ispeech.transform import *

import sys, os, time

def test():

    frame_size_in_ms = 50.0

    kake=karaoke()

    #kake.set('../../../data/midi/national anthems/China.mid','../../../data/midi/national anthems/China_mp4_audacity.wav',frame_size_in_ms,10) 
    #kake.set('../../../data/midi/national anthems/China.mid','../../../data/midi/national anthems/China.wav',frame_size_in_ms,10)
    kake.set('../../../data/midi/HappyBirthday.mid','../../../data/midi/HappyBirthday_gband_make.wav',frame_size_in_ms,23)

    #kake.set('../../../data/midi/twinkle.mid','../../../data/midi/twinkle.wav',50,10)

    wav=kake.getwav()
    midi=kake.getmidi()

    wav = np.array(wav)
    midi= np.array(midi)
    plt.plot(np.arange(wav.size)*frame_size_in_ms,wav,'r-',marker='x',label = 'wav');


    score = kake.getscore()
    plt.plot(np.arange(midi.size)*frame_size_in_ms,midi,'b-',marker='x',label= 'midi score='+str(score));

    

    plt.legend()
    plt.show()

test()

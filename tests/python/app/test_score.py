import pdb
import numpy as np 
import matplotlib.pyplot as plt
from ispeech.source import *
from ispeech.transform import *

import sys, os, time
sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__)), '../../../src/python/'))

import speech_model_sine as speech_model_sine
import speech_source as speech_source



def Autocorrelation(signal):
    a = np.zeros(signal.size)
    for k in np.arange(signal.size):
        for i in np.arange(signal.size-k):
            a[k] += signal[i]*signal[i+k]

    return a

def DetectPitch(a,fs):
    kmax = 0
    for k in np.arange(a.size):
        if a[k]< 0:
            new_a = a[k:]
            new_a = new_a.tolist()
            kmax = new_a.index(max(new_a))+k
            break

    kmax = peakInterp(a,kmax)
    return kmax,fs/kmax

def peakInterp(mX,ploc):
    """
    Interpolate peak values using parabolic interpolation
    mX, pX: magnitude and phase spectrum, ploc: locations of peaks
    returns iploc, ipmag, ipphase: interpolated peak location, magnitude and phase values
    """

    # magnitude of peak bin
    val = mX[ploc]
    # magnitude of bin at left
    lval = mX[ploc-1]
    # magnitude of bin at right
    rval = mX[ploc+1]
    # center of parabola
    iploc = ploc + 0.5*(lval-rval)/(lval-2*val+rval)

    return iploc

def test():

    frame_size_in_ms = 100
    
    fs,x=speech_source.wavread('../../../data/midi/national anthems/China.wav');

    H = int(frame_size_in_ms*fs/1000)

    x = x[:102400]
    pin = 0
    pend = x.size-H

    soudpitches = np.array([])
    while pin < pend:
        x1 = x[pin:pin+H]

        x1 = Autocorrelation(x1)

        kmax,pitch = DetectPitch(x1,fs)
        pitch = 69 + 12*np.log2(pitch/440.0)
        
        soudpitches = np.append(soudpitches,pitch)
        print('pin='+str(pin))
        
        print('H='+str(H))
        print('pitch='+str(pitch))
        pin+=H
    

    print('size = '+str(soudpitches.size))

    pitch_size = soudpitches.size

        
    mparser = midi_parser();

    pitches = mparser.parser2('../../../data/midi/national anthems/China.mid',frame_size_in_ms);

    if pitch_size > len(pitches):
        pitch_size = len(pitches)
    
    plt.subplot(2,1,1)
    pitches = np.array(pitches);

    plt.plot(np.arange(pitch_size),pitches[:pitch_size],'r-');

    
    plt.plot(np.arange(pitch_size),soudpitches[:pitch_size],'b-');

    plt.legend()
    plt.show()

test()

import pdb
import numpy as np 
import matplotlib.pyplot as plt
from scipy.signal import get_window
import math
from ispeech.source import *
from ispeech.transform import *
import sys, os, time

sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__)), '../../../src/python/'))

import speech_model_sine as speech_model_sine
import speech_source as speech_source
import speech_model_stft as speech_model_stft


def test():
    #signal param
    fs = 44100#sample rate
    f = 500#frequency of signal
    L = 1024#length to be generate signal

    #transform param
    N = 1024
    M = 800
    H = 160
    t = -80
    minSineDur = .02
    maxnSines = 100

    #generate signal and window
    sine = SineGenerator(fs)
    sine.setFrequency(f)
    sine.setAmplitude(1.0)
    sine.setPhase(1.0)
    sine.generate(L)
    hamming = HammingWindow(M)

    
    x = np.array(sine)

    # c++ analysis code test 
    ms = ModelSine()

    y = ms.AnalysisAndSynthesis(sine, fs, hamming, N, H, t, maxnSines, minSineDur,20.0,0.01)
 
    plt.figure(1, figsize=(9.5, 5))
    plt.subplot(3,1,1)
    time = np.arange(0,L/fs,1/fs)

    plt.plot(np.arange(x.size),x)
    plt.autoscale(tight=True)
    plt.title('signal')

    plt.subplot(3,1,2)

    y = np.array(y);

    plt.plot(np.arange(y.size),y,'g',label='c++ function')

    w = np.array(hamming)
    tfreq, tmag, tphase = speech_model_sine.sineModelAnal(x, fs, w, N, H, t, maxnSines, minSineDur,20.0,0.01)
    Ns = 512
    #H = Ns//4
    
    y2 = speech_model_sine.sineModelSynth(tfreq,tmag,tphase,Ns,H,fs)

    print('y2.size ='+str(y2.size))
    plt.plot(np.arange(y2.size),y2,'r',label='python function')
      
    plt.autoscale(tight=True)
    plt.title('synthesis y')
    plt.tight_layout()
    plt.legend()
    plt.show()
    
test()

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
import speech_model_dft as speech_model_dft
import speech_model_stft as speech_model_stft


def test_sineAnal():
    #signal param
    fs = 44100#sample rate
    f = 500#frequency of signal
    L = 1024#length to be generate signal

    #transform param
    N = 2048
    M = 800
    H = 160
    t = -80
    minSineDur = .02
    maxnSines = 10

    #generate signal and window
    sine = SineGenerator(fs)
    sine.setFrequency(f)
    sine.setAmplitude(1.0)
    sine.setPhase(1.0)
    sine.generate(L)
    hamming = HammingWindow(M)

    #do stft
    x = np.array(sine)
    w = np.array(hamming)
    mX, pX = speech_model_stft.stftAnal(x, w, N, H)

    # c++ analysis code test 
    ms = ModelSine()
    ms.Analysis(sine, fs, hamming, N, H, t, maxnSines, minSineDur,20.0,0.01)
    num = ms.GetAnalysisTypeCount();
    
    for i in range(num):
        AT = ms.GetAnalysisTypeByIndex(i);
        jtfreq = AT.tfreqn
        jtmag = AT.tmagn
        jtphase = AT.tphasen
        jtfreq = np.array(jtfreq)
        jtmag = np.array(jtmag)
        jtphase = np.array(jtphase)
        print('jtfreq size = '+ str(jtfreq.size))
        print('jtmag size = '+ str(jtmag.size))
        print('jtphase size = '+ str(jtphase.size))
        if i == 0:     
            tfreq = jtfreq 
            tmag = jtmag
            tphase = jtphase
        else:
            tfreq = np.vstack((tfreq, jtfreq))
            tmag = np.vstack((tmag, jtmag))
            tphase = np.vstack((tphase, jtphase))

    plt.figure(1, figsize=(9.5, 5))
    plt.subplot(3,1,1)
    time = np.arange(0,L/fs,1/fs)

    plt.plot(time,x)
    plt.autoscale(tight=True)
    plt.title('signal')
    plt.subplot(3,1,2)
    
    maxplotfreq = 5000.0
    maxplotbin = int(N*maxplotfreq/fs)
    numFrames = int(mX[:,0].size)
    frmTime = H*np.arange(numFrames)/float(fs)                             
    binFreq = np.arange(maxplotbin+1)*float(fs)/N                         
    plt.pcolormesh(frmTime, binFreq, np.transpose(mX[:,:maxplotbin+1]))
    plt.autoscale(tight=True)

    tracks = tfreq*np.less(tfreq, maxplotfreq)
    tracks[tracks<=0] = np.nan
    plt.plot(frmTime, tracks, color='k', lw=1.5)
    plt.autoscale(tight=True)
    plt.title('mX + sinusoidal tracks by c++ sine model)')

    # python code do the same test 
    plt.subplot(3,1,3)
    tfreq, tmag, tphase = speech_model_sine.sineModelAnal(x, fs, w, N, H, t, maxnSines, minSineDur,20.0,0.01)

    maxplotfreq = 5000.0
    maxplotbin = int(N*maxplotfreq/fs)
    numFrames = int(mX[:,0].size)
    frmTime = H*np.arange(numFrames)/float(fs)                             
    binFreq = np.arange(maxplotbin+1)*float(fs)/N                         
    plt.pcolormesh(frmTime, binFreq, np.transpose(mX[:,:maxplotbin+1]))
    plt.autoscale(tight=True)

    print('tfreq =')
    print(tfreq)
    
    tracks = tfreq*np.less(tfreq, maxplotfreq)
    tracks[tracks<=0] = np.nan
    print('tracks size = '+str(tracks.size))
    print('tracks = ')
    print(tracks)
    
    plt.plot(frmTime, tracks, color='k', lw=1.5)
    plt.autoscale(tight=True)
    plt.title('mX + sinusoidal same tracks by python')

    
    plt.tight_layout()
    plt.show()
    
test_sineAnal()

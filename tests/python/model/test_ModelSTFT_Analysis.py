import numpy as np 
import matplotlib.pyplot as plt
from scipy.fftpack import fft,fftshift,ifft
from scipy.signal import sawtooth
from scipy.signal import get_window
import math
import sys, os, time
sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__)), '../../../src/python/'))

import speech_model_sine as speech_model_sine
import speech_source as speech_source
import speech_model_dft as speech_model_dft
import speech_model_stft as speech_model_stft

#for c++ 
from ispeech.source import *
from ispeech.transform import *

def test_model():

    fs = 44100
    L = 1024*5;
    f = 100
    sine = SineGenerator(fs)
    sine.setFrequency(f)
    sine.setAmplitude(1.0)
    sine.setPhase(1.0)
    sine.generate(L)
    
    x = np.array(sine)
    w = np.hamming(1024)
    N = 2048
    H = 512
    mX, pX = speech_model_stft.stftAnal(x, w, N, H)
    y = speech_model_stft.stftSynth(mX, pX, w.size, H)

    #c++
    hamming = HammingWindow(1024)
    stft = ModelSTFT()

    y2 = stft.AnalysisAndSynthesis(sine,hamming,N,H)
    y2 = np.array(y2)

    plt.figure(1, figsize=(9.5, 7))
    plt.subplot(411)
    plt.plot(np.arange(x.size)/float(fs), x, 'b')
    plt.title('sinusoid f = 1000Hz')
    plt.axis([0,x.size/float(fs),min(x),max(x)])

    plt.subplot(412)
    numFrames = int(mX[:,0].size)

    print('x size = ' + str(x.size/H + 1))

    print('numFrames = ' + str(numFrames))

    print(mX.shape)

    frmTime = H*np.arange(numFrames)/float(fs)                             

    print('mX[0,:].size = '+ str(mX[0,:].size) )

    binFreq = np.arange(mX[0,:].size)*float(fs)/N                         
    print('binFreq='+str(binFreq))

    plt.pcolormesh(frmTime, binFreq, np.transpose(mX))
    plt.title('mX, M=1024, N=1024, H=512')
    plt.autoscale(tight=True)

    plt.subplot(413)
    numFrames = int(pX[:,0].size)
    frmTime = H*np.arange(numFrames)/float(fs)                             
    binFreq = np.arange(pX[0,:].size)*float(fs)/N                         
    plt.pcolormesh(frmTime, binFreq, np.diff(np.transpose(pX),axis=0))
    plt.title('pX derivative, M=1024, N=1024, H=512')
    plt.autoscale(tight=True)

    plt.subplot(414)
    plt.plot(np.arange(y.size)/float(fs), y,'b')

    plt.plot(np.arange(y2.size)/float(fs), y2,'r')
    plt.axis([0,y.size/float(fs),min(y),max(y)])
    plt.title('y')

    plt.tight_layout()
    #plt.savefig('stft-system.png')
    plt.show()


test_model()

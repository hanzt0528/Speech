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
import speech_model_harmonic as speech_model_harmonic

#for c++
from ispeech.source import *
from ispeech.transform import *

def test_model():

    fs = 44100
    M=256
    N = 512
    sine = SineGenerator(fs)
    sine.setFrequency(880)
    sine.setAmplitude(1.0)
    sine.setPhase(0.1)
    sine.generate(M)

    x = np.array(sine)
    w = np.hamming(M)
    f0 = speech_model_harmonic.f0Detection(x,fs,w,N,64,-100,400,1000,1)
    print(f0)
    plt.figure(1, figsize=(9.5, 7))
    plt.subplot(411)
    plt.plot(np.arange(x.size)/float(fs), x, 'b')
    plt.title('sinusoid f = 1000Hz')
    plt.axis([0,x.size/float(fs),min(x),max(x)])


    plt.tight_layout()
    plt.show()

def test_f0Detection():
      (fs, x) = speech_source.wavread('../../../data/vignesh.wav')

      x = x[int(1.6*fs):int(1.7*fs)]

      s = SignalSource(x.size)


      for i in range(0,x.size):
          s.set(i,x[i])

      x = s
      harmonic = ModelHarmonic()


      N = 2048
      t = -90
      minf0 = 100
      maxf0 = 300
      f0et = 3
      maxnpeaksTwm = 10
      H = 128

      w = HammingWindow(1501)


      #c++ backend
      cf0 = harmonic.f0Detection(x, fs, w, N, H, t, minf0, maxf0, f0et)
      cf0 = np.array(cf0)
      cf0 = speech_model_sine.cleaningTrack(cf0, 5)
      cyf0 = speech_model_sine.sinewaveSynth(cf0, .8, H, fs)
      cf0[cf0==0] = np.nan


      #python backend
      x = np.array(x)
      w = np.array(w)
      mX, pX = speech_model_stft.stftAnal(x, w, N, H)
      f0 = speech_model_harmonic.f0Detection(x, fs, w, N, H, t, minf0, maxf0, f0et)
      f0 = speech_model_sine.cleaningTrack(f0, 5)
      yf0 = speech_model_sine.sinewaveSynth(f0, .8, H, fs)
      f0[f0==0] = np.nan


      plt.figure(1, figsize=(9, 7))

      plt.subplot(2,1,1)
      maxplotfreq = 800.0
      numFrames = int(mX[:,0].size)
      frmTime = H*np.arange(numFrames)/float(fs)
      binFreq = fs*np.arange(N*maxplotfreq/fs)/N
      plt.pcolormesh(frmTime, binFreq, np.transpose(mX[:,:int(N*maxplotfreq/fs+1)]))
      plt.autoscale(tight=True)

      plt.plot(frmTime, f0, linewidth=2, color='k',marker = 'x')
      #plt.plot(frmTime, cf0, linewidth=2, color='r')
      plt.autoscale(tight=True)
      plt.title('mX + f0 (piano.wav), TWM-MY python')

      plt.subplot(2,1,2)
      plt.pcolormesh(frmTime, binFreq, np.transpose(mX[:,:int(N*maxplotfreq/fs+1)]))
      plt.autoscale(tight=True)
      plt.plot(frmTime, cf0, linewidth=2, color='r',marker = 'x')
      plt.autoscale(tight=True)
      plt.title('mX + f0 (piano.wav), TWM-MY c++')

      plt.tight_layout()
      #plt.savefig('f0Twm-piano.png')
      #speech_source.wavwrite(yf0, fs, 'f0Twm-piano.wav')
      plt.show()


#test_model()
test_f0Detection()

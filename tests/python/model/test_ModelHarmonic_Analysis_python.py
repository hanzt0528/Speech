import numpy as np
import matplotlib.pyplot as plt
from ispeech.source import *
from ispeech.transform import *
import sys, os, time

sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__)), '../../../src/python/'))

import speech_model_sine as speech_model_sine
import speech_source as speech_source
import speech_model_dft as speech_model_dft
import speech_model_stft as speech_model_stft
import speech_model_harmonic as speech_model_harmonic



def test_sineAnal():

  (fs, x) = speech_source.wavread('../../../data/vignesh.wav')
  x = x[int(1.6*fs):int(1.7*fs)]
  w = np.hamming(1201)
  N = 2048
  t = -90
  nH = 100
  minf0 = 130
  maxf0 = 300
  f0et = 7
  Ns = 512
  H = Ns//4
  minSineDur = .1
  harmDevSlope = 0.01
  hfreq, hmag, hphase = speech_model_harmonic.harmonicModelAnal(x, fs, w, N, H, t, nH, minf0, maxf0, f0et, harmDevSlope, minSineDur)
  y = speech_model_sine.sineModelSynth(hfreq, hmag, hphase, Ns, H, fs)

  numFrames = int(hfreq[:,0].size)
  frmTime = H*np.arange(numFrames)/float(fs)

  plt.figure(1, figsize=(9, 7))

  plt.title('harmonic analysis implement in python')
  plt.subplot(3,1,1)
  plt.plot(np.arange(x.size)/float(fs), x, 'b')
  plt.axis([0,x.size/float(fs),min(x),max(x)])
  plt.title('x (vignesh.wav)')

  plt.subplot(3,1,2)
  yhfreq = hfreq
  yhfreq[hfreq==0] = np.nan
  plt.plot(frmTime, hfreq, lw=1.2)
  #plt.axis([0,y.size/float(fs),0,8000])
  plt.title('f_h, harmonic frequencies')

  plt.subplot(3,1,3)
  plt.plot(np.arange(y.size)/float(fs), y, 'b')
  plt.title('yh')

  plt.tight_layout()
  #speech_source.wavwrite(y, fs, 'vignesh-harmonic-synthesis.wav')
  #plt.savefig('harmonicModel-analysis-synthesis.png')
  plt.show()

test_sineAnal()

import math
import matplotlib.pyplot as plt
import numpy as np
import time, os, sys

sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__)), '../../../src/python/'))

import speech_model_dft as speech_model_dft

import speech_source as speech_source

(fs, x) =  speech_source.wavread('../../../data/orchestra.wav')
N = 2048
start = int(1.0*fs)
x1 = x[start:start+N]

plt.figure(1, figsize=(12, 9))
plt.subplot(321)
plt.plot(np.arange(N)/float(fs), x1*np.hamming(N), 'b', lw=1.5)
plt.axis([0, N/float(fs), min(x1*np.hamming(N)), max(x1*np.hamming(N))])
plt.title('x (orchestra.wav)')

mX, pX = speech_model_dft.dftAnal(x1, np.hamming(N), N)
startBin = int(N*500.0/fs)
nBins = int(N*4000.0/fs)
bandpass = (np.hanning(nBins) * 100.0) - 100

filt = np.zeros(mX.size)-100
filt[startBin:startBin+nBins] = bandpass
mY = mX + filt

plt.subplot(323)

plt.plot(fs*np.arange(mX.size)/N, mX, 'r', lw=1.5, label = 'mX')
plt.plot(fs*np.arange(mX.size)/N, filt+max(mX), 'g', lw=1.5, label='filter')

plt.plot(fs*np.arange(mX.size)/N, mY, 'k', lw=1.5, label='filter')

plt.legend(prop={'size':10})
plt.axis([0,fs/4.0,-90,max(mX)+2])
plt.title('mX + filter')

plt.subplot(325)
plt.plot(fs*np.arange(pX.size)/N, pX, 'c', lw=1.5)
plt.axis([0,fs/4.0,min(pX),8])
plt.title('pX')

y = speech_model_dft.dftSynth(mY, pX, N)*sum(np.hamming(N))
mY1, pY = speech_model_dft.dftAnal(y, np.hamming(N), N)
plt.subplot(322)
plt.plot(np.arange(N)/float(fs), y, 'b')
plt.axis([0, float(N)/fs, min(y), max(y)])
plt.title('y')

plt.subplot(324)
plt.plot(fs*np.arange(mY1.size)/N, mY1, 'r', lw=1.5)
plt.axis([0,fs/4.0,-90,max(mY1)+2])
plt.title('mY')

plt.subplot(326)
plt.plot(fs*np.arange(pY.size)/N, pY, 'c', lw=1.5)
plt.axis([0,fs/4.0,min(pY),8])
plt.title('pY')

plt.tight_layout()
plt.savefig('FFT-filtering.png')

speech_source.wavwrite(y,fs,"test_fft_filtering.wav")

plt.show()

import pdb
import numpy as np 
import matplotlib.pyplot as plt
from ispeech.source import *
from ispeech.transform import *

def peakDetection(mX,t):
    size = len(mX)
    ploc = np.zeros(size,dtype=int)
    j = 0;
    for i in range(1,size-1):
        if mX[i] > t:
            if mX[i]>mX[i-1] and mX[i] > mX[i+1]:
                ploc[j] = int(i)
                j+=1

    return ploc;
def peakDetection2(mX, t):
    thresh = np.where(np.greater(mX[1:-1],t), mX[1:-1], 0) 
    next_minor = np.where(mX[1:-1]>mX[2:], mX[1:-1], 0)   
    prev_minor = np.where(mX[1:-1]>mX[:-2], mX[1:-1], 0)
    ploc = thresh * next_minor * prev_minor
    ploc = ploc.nonzero()[0] + 1
    return ploc

    
def test_peakDetection():
  
    fs = 44100
    f = 500
    N = 256;
    sine = SineGenerator(fs)
    sine.setFrequency(f)
    sine.setAmplitude(1.0)
    sine.setPhase(1.0)
    sine.generate(N)

    hamming = HammingWindow(N)

    sine = np.array(sine)
    hamming = np.array(hamming)

    sine = sine*hamming
    
    dft = Dft(N)
   
    spectrum_sine=dft.fft(sine)

    spectrum_sine = np.array(spectrum_sine);

   
    sine_idft = dft.ifft(spectrum_sine)

    plt.figure(1)
    plt.subplot(2,1,1)
    plt.plot(np.arange(N),sine)
    ploc = peakDetection2(sine,-100)
    
    plt.plot(ploc, sine[ploc], marker='x', color='b', linestyle='', markeredgewidth=1.5)   
    plt.title('sine signal')

    plt.subplot(2,1,2)
    plt.plot(np.arange(spectrum_sine.size)*fs/N,abs(spectrum_sine),'g',label='spectrum_sine')
    ploc = peakDetection2(abs(spectrum_sine),-100)
    plt.plot(ploc*fs/N, abs(spectrum_sine[ploc]), marker='x', color='b', linestyle='', markeredgewidth=1.5)   

    plt.title('DFT')

    plt.legend()
    plt.show()

test_peakDetection()

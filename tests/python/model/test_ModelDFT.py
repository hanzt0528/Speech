import pdb
import numpy as np 
import matplotlib.pyplot as plt
from ispeech.source import *
from ispeech.transform import *
import math
def test_dftModel():
  
    fs = 44100#sample rate
    f = 500#frequency of signal
    M = 256#signal size
    N = 512#FFT size
    sine = SineGenerator(fs)
    sine.setFrequency(f)
    sine.setAmplitude(1.0)
    sine.setPhase(1.0)
    sine.generate(M)

    dftModel = ModelDFT()
    hamming = HammingWindow(M)

    #DFT Model the signal
    y = dftModel.dftModel(sine,hamming,N)


    #show windowed signal by hamming window
    x = np.array(sine)
    w = np.array(hamming)
    xw = x*w
    plt.figure(1)
    plt.subplot(3,1,1)
    plt.plot(np.arange(xw.size),xw)
    plt.title('windowed sine signal')


    #show zero-padding signal
    hN = (N//2)+1  
    hM1 = (M+1)//2            
    hM2 = int(math.floor(M/2))

    print(hN)
    print(hM1)
    print(hM2)
    fftbuffer = np.zeros(N)         
    fftbuffer[:hM1] = xw[hM2:]
    fftbuffer[-hM2:] = xw[:hM2]
    
    plt.subplot(3,1,2)
    plt.plot(np.arange(fftbuffer.size),fftbuffer,'r')
    plt.title('zero-padding signal')

    #show mode synthesis signal
    plt.subplot(3,1,3)
    plt.plot(np.arange(len(y)),y,'g')
    plt.title('use model synthesis signal')

    print(np.finfo(float).eps)

    plt.show()
    
test_dftModel()

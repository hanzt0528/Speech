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
    L = 256*10
    sine = SineGenerator(fs)
    sine.setFrequency(f)
    sine.setAmplitude(1.0)
    sine.setPhase(1.0)
    sine.generate(L)

    model = ModelSine()
    hamming = HammingWindow(M)

    #DFT Model the signal
    y = model.sineModel(sine,fs,hamming,N,-20.0)
    

    #show windowed signal by hamming window
    x = np.array(sine)
    plt.figure(1)
    plt.subplot(2,1,1)
    plt.plot(np.arange(x.size),x)
    plt.title('sine signal frequency= '+str(f)+' Hz')


    #show mode synthesis signal
    plt.subplot(2,1,2)
    plt.plot(np.arange(len(y)),y,'g')
    plt.title('use ModelSine(c++) and  hamming window to synthesis signal')

    plt.show()
    
test_dftModel()

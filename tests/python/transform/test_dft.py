import pdb
import numpy as np 
import matplotlib.pyplot as plt
from ispeech.source import *
from ispeech.transform import *

def test_dft():
  
    fs = 600
    f = 120
    N = 60;
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
    plt.subplot(3,1,1)
    plt.plot(np.arange(N),sine)
    plt.title('sine signal')

    plt.subplot(3,1,2)
    plt.plot(np.arange(spectrum_sine.size)*fs/N,abs(spectrum_sine),'g',label='spectrum_sine')
    plt.title('DFT')


    plt.subplot(3,1,3)
    plt.plot(np.arange(N),sine_idft)
    plt.title('IDFT')

    

    plt.legend()
    plt.show()

test_dft()

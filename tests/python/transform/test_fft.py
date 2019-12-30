import pdb
import numpy as np 
import matplotlib.pyplot as plt
from ispeech.source import *
from ispeech.transform import *


def test_fft():

  
    fs = 44100
    f = 500
    N = 256;
    sine = SineGenerator(fs)
    sine.setFrequency(f)
    sine.setAmplitude(1.0)
    sine.setPhase(1.0)
    sine.generate(N)

    fft = OouraFft(N)
   
    spectrum_sine=fft.fft(sine)

    spectrum_sine = np.array(spectrum_sine);

   
    sine_ifft = fft.ifft(spectrum_sine)

    plt.figure(1)
    plt.subplot(3,1,1)
    plt.plot(np.arange(N),sine)
    plt.title('sine signal')

    plt.subplot(3,1,2)
    plt.plot(np.arange(spectrum_sine.size)*fs/N,abs(spectrum_sine),'g',label='spectrum_sine')
    plt.title('FFT')


    plt.subplot(3,1,3)
    plt.plot(np.arange(N),sine_ifft)
    plt.title('IFFT')

    

    plt.legend()
    plt.show()

    #test_hamming_window()
test_fft()

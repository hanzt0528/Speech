import pdb
import numpy as np 
import matplotlib.pyplot as plt
from ispeech.source import *
from ispeech.transform import *


def test_fft():

    #here are two signals, one frequency is 500Hz,other is 1000
    fs = 44100
    f1 = 500
    f2 = 1000
    N = 256;

    #get a FFT
    fft = OouraFft(N)

    #signal1 FFT and IFFT
    signal1 = SineGenerator(fs)
    signal1.setFrequency(f1)
    signal1.setAmplitude(1.0)
    signal1.setPhase(1.0)
    signal1.generate(N)
    spectrum_signal1=fft.fft(signal1)
    spectrum_signal1 = np.array(spectrum_signal1);   
    signal1_ifft = fft.ifft(spectrum_signal1)

    #signal2 FFT and IFFT
    signal2 = SineGenerator(fs)
    signal2.setFrequency(f2)
    signal2.setAmplitude(1.0)
    signal2.setPhase(1.0)
    signal2.generate(N)
    spectrum_signal2=fft.fft(signal2)
    spectrum_signal2 = np.array(spectrum_signal2);   
    signal2_ifft = fft.ifft(spectrum_signal2)

    #get FFT of sum of both signals.
    spectrum_of_signal1_plus_signal2=fft.fft(np.array(signal1)+np.array(signal2))

    #draw the sum of both signals.
    plt.figure(1)
    plt.subplot(4,1,1)
    plt.plot(np.arange(N),np.array(signal1)+np.array(signal2))
    plt.title('signal1+signal2')
    plt.legend()

    #draw the each one of the spectrum of the signals.
    plt.subplot(4,1,2)
    plt.plot(np.arange(spectrum_signal1.size)*fs/N,abs(spectrum_signal1),'r',label='spectrum_signal1')
    plt.plot(np.arange(spectrum_signal2.size)*fs/N,abs(spectrum_signal2),'g',label='spectrum_signal2')

    plt.title('FFT')
    plt.legend()


    #draw the  spectral sum  of two signals.
    plt.subplot(4,1,3)
    plt.plot(np.arange(spectrum_signal2.size)*fs/N,abs(spectrum_signal1+spectrum_signal2),'g',label='spectrum_signal1 + spectrum_signal2')
    plt.title('FFT')
    plt.legend()


    #draw the spectrum of sum of two signals.
    plt.subplot(4,1,4)
    plt.plot(np.arange(spectrum_signal2.size)*fs/N,abs(np.array(spectrum_of_signal1_plus_signal2)),'g',label='spectrum(signal1+signal2)')
    plt.title('FFT')
    plt.legend()

    
    plt.show()

    #test_hamming_window()
test_fft()

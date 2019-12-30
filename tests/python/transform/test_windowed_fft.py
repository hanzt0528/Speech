import pdb
import numpy as np 
import matplotlib.pyplot as plt
from ispeech.source import *
from ispeech.transform import *

def test_hamming_window():
    hamming = HammingWindow(100)
    hamming = np.array(hamming)
    plt.plot(np.arange(hamming.size),hamming)
    plt.show()

def test_multiply_sine_and_hamming():

    sine = SineGenerator(44100)
    sine.setFrequency(440.0)
    sine.setAmplitude(1.0)
    sine.setPhase(1.0)
    sine.generate(1000)

    sine_windowed = sine
    hamming = HammingWindow(1000)

    sine_windowed = np.array(sine_windowed)
    hamming = np.array(hamming)
    sine_windowed*=hamming

    plt.title('sine windowed signal (f= 440.0Hz and hamming)')
    plt.plot(np.arange(sine_windowed.size),sine_windowed,'g')
    plt.show()

def test_fft():

    N = 256;
    sine = SineGenerator(44100)
    sine.setFrequency(5000.0)
    sine.setAmplitude(1.0)
    sine.setPhase(1.0)
    sine.generate(N)

    fft = OouraFft(N)
   
    sine_windowed = sine
    hamming = HammingWindow(N)

    sine_windowed = np.array(sine_windowed)
    hamming = np.array(hamming)
    sine_windowed*=hamming

    spectrum_sine=fft.fft(sine)

    spectrum_sine = np.array(spectrum_sine);
    plt.plot(np.arange(spectrum_sine.size),abs(spectrum_sine),'g',label='spectrum_sine')

    spectrum_sine_windowed=fft.fft(sine_windowed)

    spectrum_sine_windowed = np.array(spectrum_sine_windowed);
    plt.plot(np.arange(spectrum_sine_windowed.size),abs(spectrum_sine_windowed),'r',label='spectrum_sine_windowed')

    plt.legend()
    plt.show()

    #test_hamming_window()
test_fft()

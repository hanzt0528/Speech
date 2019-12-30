import pdb
import numpy as np
import matplotlib.pyplot as plt
from ispeech.source import *


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

test_multiply_sine_and_hamming()

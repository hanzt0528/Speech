import pdb
import numpy as np 
import matplotlib.pyplot as plt
from ispeech.source import *


def test_windows():
    hann = HannWindow(100)
    hann = np.array(hann)
    plt.plot(np.arange(hann.size),hann,label='hann')


    hamming = HammingWindow(100)
    hamming = np.array(hamming)
    plt.plot(np.arange(hamming.size), hamming,label= 'hamming')


    Gaussian = GaussianWindow(100,0.5)
    Gaussian = np.array(Gaussian)
    plt.plot(np.arange(Gaussian.size), Gaussian,label= 'Gaussian')


    Flattop = FlattopWindow(100)
    Flattop = np.array(Flattop)
    plt.plot(np.arange(Flattop.size), Flattop,label= 'Flattop')
    
 
    Barlett = BarlettWindow(100)
    Barlett = np.array(Barlett)
    plt.plot(np.arange(Barlett.size), Barlett,label= 'Barlett')

    
    plt.legend()
    plt.show()

test_windows()

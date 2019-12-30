import pdb
import numpy as np
import matplotlib.pyplot as plt
from ispeech.source import *


def test_hamming_window():
    hamming = HammingWindow(100)
    hamming = np.array(hamming)
    plt.plot(np.arange(hamming.size),hamming)
    plt.show()

test_hamming_window()

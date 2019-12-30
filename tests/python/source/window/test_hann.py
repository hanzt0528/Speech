import pdb
import numpy as np 
import matplotlib.pyplot as plt
from ispeech.source import *


def test_hann_window():
    hann = HannWindow(100)
    hann = np.array(hann)

    print(sum(hann))
    plt.plot(np.arange(hann.size),hann)
    plt.show()


test_hann_window()

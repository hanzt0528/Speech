import pdb
import numpy as np 
import matplotlib.pyplot as plt
from ispeech.source import *
from scipy.signal import blackmanharris, triang


def test_window():

    H = 100
    window1 = blackmanharris(2*H)
    window2 = BlackmanharrisWindow(2*H)
    plt.plot(np.arange(len(window1)),window1,'g',label='scipy blackmanharris')
    plt.plot(np.arange(len(window2)),window2,'y',label = 'ispeech blackmanharris')

    print(sum(window1))
    print('--------')
    window2 = np.array(window2)
    print(sum(window2))

    plt.legend()
    plt.show()


test_window()

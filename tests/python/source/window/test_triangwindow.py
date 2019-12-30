import pdb
import numpy as np 
import matplotlib.pyplot as plt
from ispeech.source import *
from scipy.signal import blackmanharris, triang


def test_triang_window():

    H = 100
    triangle = triang(2*H)
    triangle2 = TriangWindow(2*H)
    plt.plot(np.arange(triangle.size),triangle,'g',label='scipy triang')
    plt.plot(np.arange(len(triangle2)),triangle2,'y',label = 'ispeech triang')

    print(sum(triangle))
    print('--------')
    triangle2 = np.array(triangle2)
    print(sum(triangle2))

    plt.legend()
    plt.show()


test_triang_window()

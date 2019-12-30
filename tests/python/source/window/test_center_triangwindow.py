import pdb
import numpy as np 
import matplotlib.pyplot as plt
from scipy.signal import blackmanharris, triang


def test():

    Ns = 512
    H = Ns//4
    hNs = Ns//2
    triangle = triang(2*H)

    plt.plot(np.arange(triangle.size),triangle,'g')
    
    sw = np.zeros(Ns)
    sw[hNs-H:hNs+H] = triangle

    sw2 = np.zeros(Ns)

    j = 0
    for i in range(hNs-H,hNs+H):
        sw2[i]=triangle[j]
        j+=1
   
    plt.plot(np.arange(sw.size),sw,'r',label = 'center triangular');
    plt.plot(np.arange(sw2.size),sw2,'y',label = 'center triangular');

    plt.legend()
    plt.show()

test()

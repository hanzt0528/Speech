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
    plt.legend()
    
    sw = np.zeros(Ns)
    sw[hNs-H:hNs+H] = triangle


    plt.figure(1)
    plt.subplot(3,1,1)
    plt.plot(np.arange(sw.size),sw,'r',label = 'center triangular');
    plt.legend()


    bh = blackmanharris(Ns)

    bh=bh/sum(bh)

    print(sum(bh))
    plt.subplot(3,1,2)
    plt.plot(np.arange(bh.size),bh,'r',label = 'normalized blackmanharris');
    plt.legend()
    

    sw[hNs-H:hNs+H]= sw[hNs-H:hNs+H]/bh[hNs-H:hNs+H]

    print(sum(sw))
    plt.subplot(3,1,3)
    plt.plot(np.arange(sw.size),sw,'r',label = 'normalized blackmanharris');
    

    plt.legend()
    plt.show()

test()

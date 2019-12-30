import pdb
import numpy as np 
import matplotlib.pyplot as plt
from ispeech.source import *
from ispeech.transform import *

def peakDetection(mX,t):
    size = len(mX)
    ploc = np.zeros(size,dtype=int)
    j = 0;
    for i in range(1,size-1):
        if mX[i] > t:
            if mX[i]>mX[i-1] and mX[i] > mX[i+1]:
                ploc[j] = int(i)
                j+=1

    return ploc;
def peakDetection2(mX, t):
    thresh = np.where(np.greater(mX[1:-1],t), mX[1:-1], 0) 
    next_minor = np.where(mX[1:-1]>mX[2:], mX[1:-1], 0)   
    prev_minor = np.where(mX[1:-1]>mX[:-2], mX[1:-1], 0)
    ploc = thresh * next_minor * prev_minor
    ploc = ploc.nonzero()[0] + 1
    return ploc

#parabola
def peakInterp(mX, pX, ploc):
    val = mX[ploc]
    lval = mX[ploc-1]
    rval = mX[ploc+1]
    iploc = ploc + 0.5*(lval-rval)/(lval-2*val+rval)
    ipmag = val - 0.25*(lval-rval)*(iploc-ploc)
    ipphase = np.interp(iploc, np.arange(0, pX.size), pX)
    return iploc, ipmag, ipphase

#this function same to np.interp
def Interp(x,xp,fp):
    ix = np.zeros(len(x))
    for i in range(len(x)):
        if x[i] <= xp[0]:
            ix[i]= fp[0]
            continue
        if x[i] >= xp[-1]:
            ix[i]=fp[-1]         
        for j in range(len(xp)-1):
            if x[i] > xp[j] and x[i] < xp[j+1]:
                ix[i]= fp[j]+(x[i]-xp[j])/(xp[j+1]-xp[j])*(fp[j+1]-fp[j])
                continue
            if x[i] == xp[j]:
                ix[i]=fp[j]
                continue
    return ix


def sineModel(x, fs, w, N, t):
    # half analysis window size by rounding
    hM1 = int(math.floor((w.size+1)/2))
    # half analysis window size by floor
    hM2 = int(math.floor(w.size/2))
    # FFT size for synthesis (even)    
    Ns = 512

    # Hop size used for analysis and synthesis
    H = Ns//4
    print('H='+str(H))
    # half of synthesis FFT size
    hNs = Ns//2
    print('hNs = ' + str(hNs))

    # init sound pointer in middle of anal window
    pin = max(hNs, hM1)
    # last sample to start a frame       
    pend = x.size - max(hNs, hM1)
    # initialize buffer for FFT
    fftbuffer = np.zeros(N)
    # initialize output sound frame
    yw = np.zeros(Ns)

    # initialize output array
    y = np.zeros(x.size)
    # normalize analysis window
    w = w / sum(w)
        
    # initialize synthesis window
    sw = np.zeros(Ns)

    # triangular window
    ow = triang(2*H)

    # add triangular window
    sw[hNs-H:hNs+H] = ow

    # blackmanharris window
    bh = blackmanharris(Ns)
    # normalized blackmanharris window
    bh = bh / sum(bh)

    # normalized synthesis window
    sw[hNs-H:hNs+H] = sw[hNs-H:hNs+H] / bh[hNs-H:hNs+H]
    # while input sound pointer is within sound
    while pin<pend:             
        #-----analysis-----
        # select frame
        x1 = x[pin-hM1:pin+hM2]
        # compute dft
        mX, pX = self.dftAnal(x1, w, N)
        # detect locations of peaks
        ploc = peakDetection(mX, t)

        iploc, ipmag, ipphase = peakInterp(mX, pX, ploc)
        # convert peak locations to Hertz
        ipfreq = fs*iploc/float(N)
        #-----synthesis-----
        # generate sines in the spectrum
        Y = self.genSpecSines(ipfreq, ipmag, ipphase, Ns, fs)
        print('Y size = ' + str(Y.size))
        print(np.real(Y))
        # compute inverse FFT
        fftbuffer = np.real(ifft(Y))
        # undo zero-phase window
        yw[:hNs-1] = fftbuffer[hNs+1:]            
        yw[hNs-1:] = fftbuffer[:hNs+1]
        # overlap-add and apply a synthesis window
        #y[pin-hNs:pin+hNs] += sw*yw
        y[pin-hNs:pin+hNs] += sw*yw


        # advance sound pointer
        pin += H

    return y

xp=[1,2,3]
fp=[3,2,0]
x=[0,1,1.57,2.5,3.14]
ix=np.interp(x,xp,fp)
ix2=Interp(x,xp,fp)
print(ix)
print(ix2)

                
def test_peakInterp():
  
    fs = 44100
    f = 500
    N = 256;
    sine = SineGenerator(fs)
    sine.setFrequency(f)
    sine.setAmplitude(1.0)
    sine.setPhase(1.0)
    sine.generate(N)

    hamming = HammingWindow(N)

    sine = np.array(sine)
    hamming = np.array(hamming)

    sine = sine*hamming
    
    dft = Dft(N)
   
    spectrum_sine=dft.fft(sine)

    spectrum_sine = np.array(spectrum_sine);

    spectrum_sine = abs(spectrum_sine)
    sine_idft = dft.ifft(spectrum_sine)

    plt.figure(1)
    plt.subplot(3,1,1)
    plt.plot(np.arange(N),sine)
    ploc = peakDetection2(sine,-100)
    
    plt.plot(ploc, sine[ploc], marker='x', color='b', linestyle='', markeredgewidth=1.5)   
    plt.title('sine signal and peaks')

    plt.subplot(3,1,2)
    plt.plot(np.arange(spectrum_sine.size)*fs/N,abs(spectrum_sine),'g',label='spectrum_sine')
    ploc = peakDetection2(abs(spectrum_sine),-100)
    plt.plot(ploc*fs/N, abs(spectrum_sine[ploc]), marker='x', color='b', linestyle='', markeredgewidth=1.5)   

    plt.title('DFT and peaks')

    plt.subplot(3,1,3)
    plt.plot(np.arange(spectrum_sine.size)*fs/N,abs(spectrum_sine),'g',label='spectrum_sine')

    iploc,ipmag,ipphase = peakInterp(spectrum_sine,spectrum_sine,ploc)
    iploc = np.array(iploc,dtype=int)
    plt.plot(iploc*fs/N, abs(ipphase), marker='x', color='b', linestyle='', markeredgewidth=1.5)
    print(iploc.size)

    print(ipphase.size)
    

    plt.legend()
    plt.show()

    
def test_peakDetection():
  
    fs = 44100
    f = 500
    N = 256;
    sine = SineGenerator(fs)
    sine.setFrequency(f)
    sine.setAmplitude(1.0)
    sine.setPhase(1.0)
    sine.generate(N)

    hamming = HammingWindow(N)

    sine = np.array(sine)
    hamming = np.array(hamming)

    sine = sine*hamming
    
    dft = Dft(N)
   
    spectrum_sine=dft.fft(sine)

    spectrum_sine = np.array(spectrum_sine);

   
    sine_idft = dft.ifft(spectrum_sine)

    plt.figure(1)
    plt.subplot(2,1,1)
    plt.plot(np.arange(N),sine)
    ploc = peakDetection2(sine,-100)
    
    plt.plot(ploc, sine[ploc], marker='x', color='b', linestyle='', markeredgewidth=1.5)   
    plt.title('sine signal')

    plt.subplot(2,1,2)
    plt.plot(np.arange(spectrum_sine.size)*fs/N,abs(spectrum_sine),'g',label='spectrum_sine')
    ploc = peakDetection2(abs(spectrum_sine),-100)
    plt.plot(ploc*fs/N, abs(spectrum_sine[ploc]), marker='x', color='b', linestyle='', markeredgewidth=1.5)   

    plt.title('DFT')

    plt.legend()
    plt.show()

#test_peakDetection()
#test_peakInterp()

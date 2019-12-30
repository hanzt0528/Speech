import numpy as np 
import matplotlib.pyplot as plt
from scipy.fftpack import fft,fftshift,ifft
from scipy.signal import sawtooth
import speech_model_stft as speech_model_stft
from scipy.signal import get_window
import math
import speech_source as speech_source
def test_model():

    fs = 44100
    x = speech_source.gensine(1024*5)
    w = np.hamming(1024)
    N = 2048
    H = 512
    mX, pX = speech_model_stft.stftAnal(x, w, N, H)
    y = speech_model_stft.stftSynth(mX, pX, w.size, H)

    plt.figure(1, figsize=(9.5, 7))
    plt.subplot(411)
    plt.plot(np.arange(x.size)/float(fs), x, 'b')
    plt.title('sinusoid f = 1000Hz')
    plt.axis([0,x.size/float(fs),min(x),max(x)])

    plt.subplot(412)
    numFrames = int(mX[:,0].size)

    print('x size = ' + str(x.size/H + 1))

    print('numFrames = ' + str(numFrames))

    print(mX.shape)

    frmTime = H*np.arange(numFrames)/float(fs)                             

    print('mX[0,:].size = '+ str(mX[0,:].size) )

    binFreq = np.arange(mX[0,:].size)*float(fs)/N                         
    print('binFreq='+str(binFreq))

    plt.pcolormesh(frmTime, binFreq, np.transpose(mX))
    plt.title('mX, M=1024, N=1024, H=512')
    plt.autoscale(tight=True)

    plt.subplot(413)
    numFrames = int(pX[:,0].size)
    frmTime = H*np.arange(numFrames)/float(fs)                             
    binFreq = np.arange(pX[0,:].size)*float(fs)/N                         
    plt.pcolormesh(frmTime, binFreq, np.diff(np.transpose(pX),axis=0))
    plt.title('pX derivative, M=1024, N=1024, H=512')
    plt.autoscale(tight=True)

    plt.subplot(414)
    plt.plot(np.arange(y.size)/float(fs), y,'b')
    plt.axis([0,y.size/float(fs),min(y),max(y)])
    plt.title('y')

    plt.tight_layout()
    #plt.savefig('stft-system.png')
    plt.show()


test_model()

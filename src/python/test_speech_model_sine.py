import pdb
import numpy as np 
import matplotlib.pyplot as plt
from scipy.signal import get_window
import math
import speech_model_sine as speech_model_sine
import speech_source as speech_source
import speech_model_stft as speech_model_stft
def test_Interp():
    xp=[1,2,3]
    fp=[3,2,0]
    x=[0,1,1.57,2.5,3.14]
    ix=np.interp(x,xp,fp)
    ix2=speech_model_sine.Interp(x,xp,fp)
    print(ix)
    print(ix2)

def test_sineModel():
    fs = 44100#sample rate
    f = 500#frequency of signal
    M = 256#signal size
    N = 512#FFT size
    L = 256*3
    
    sine = speech_source.gensine(L,f,fs)    
    hamming = get_window('hamming',M)
    y  = speech_model_sine.sineModel(sine,fs,hamming,N,-100.0)

    #show windowed signal by hamming window
    x = np.array(sine)
    plt.figure(1)
    plt.subplot(2,1,1)
    plt.plot(np.arange(x.size),x)
    plt.title('windowed sine signal')


    #show mode synthesis signal
    plt.subplot(2,1,2)
    plt.plot(np.arange(len(y)),y,'g')
    plt.title('use model synthesis signal')

    plt.show()

def test_sineAnal():
    
    (fs, x) = speech_source.wavread('../../data/flute-A4.wav')
    w = np.blackman(601)
    N = 1024
    H = 150
    t = -80
    minSineDur = .1
    maxnSines = 150
    #x=x
    mX, pX = speech_model_stft.stftAnal(x, w, N, H)
    tfreq, tmag, tphase = speech_model_sine.sineModelAnal(x, fs, w, N, H, t, maxnSines, minSineDur)

    print('tfreq size = ' + str(tfreq.size))
    plt.figure(1, figsize=(9.5, 5))
    maxplotfreq = 5000.0
    maxplotbin = int(N*maxplotfreq/fs)
    numFrames = int(mX[:,0].size)
    frmTime = H*np.arange(numFrames)/float(fs)                             
    binFreq = np.arange(maxplotbin+1)*float(fs)/N                         
    plt.pcolormesh(frmTime, binFreq, np.transpose(mX[:,:maxplotbin+1]))
    plt.autoscale(tight=True)

    tracks = tfreq*np.less(tfreq, maxplotfreq)
    tracks[tracks<=0] = np.nan
    plt.plot(frmTime, tracks, color='k', lw=1.5)
    plt.autoscale(tight=True)
    plt.title('mX + sinusoidal tracks (flute-A4.wav)')

    plt.tight_layout()
    #plt.savefig('sineModelAnal-flute.png')
    plt.show()

#test_Interp()
#test_sineModel()
test_sineAnal()

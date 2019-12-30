import numpy as np
import math
import matplotlib.pyplot as plt
from scipy.signal import triang, resample,blackmanharris
from scipy.fftpack import fft,fftshift,ifft

import speech_model_dft as speech_model_dft
import speech_fft as speech_fft
  
def peakDetection(mX,t):
    size = len(mX)
    ploc = np.array([],dtype=int)
    j = 0;
    for i in range(1,size-1):
        if mX[i] > t:
            if mX[i]>mX[i-1] and mX[i] > mX[i+1]:
                ploc = np.append(ploc,int(i))
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

def sinc(x, N):
    """
    Generate the main lobe of a sinc function (Dirichlet kernel)
    x: array of indexes to compute; N: size of FFT to simulate
    returns y: samples of the main lobe of a sinc function
    """

    # compute the sinc function
    y = np.sin(N * x/2) / np.sin(x/2)
    # avoid NaN if x == 0
    y[np.isnan(y)] = N
    return y

def genBhLobe(x):
    """
    Generate the main lobe of a Blackman-Harris window
    x: bin positions to compute (real values)
    returns y: main lobe os spectrum of a Blackman-Harris window
    """

    # size of fft to use
    N = 512
    # frequency sampling
    f = x*np.pi*2/N
    df = 2*np.pi/N
    # initialize window
    y = np.zeros(x.size)
    # window constants
    consts = [0.35875, 0.48829, 0.14128, 0.01168]
    # iterate over the four sincs to sum
    for m in range(0,4):
        # sum of scaled sinc functions
        y += consts[m]/2 * (sinc(f-df*m, N) + sinc(f+df*m, N))
    # normalize
    y = y/N/consts[0]
    return y
    
def genSpecSines(ipfreq, ipmag, ipphase, N, fs):
    """
    Generate a spectrum from a series of sine values
    iploc, ipmag, ipphase: sine peaks locations, magnitudes and phases
    N: size of the complex spectrum to generate; fs: sampling rate
    returns Y: generated complex spectrum of sines
    """

    # initialize output complex spectrum
    Y = np.zeros(N, dtype = complex)
    # size of positive freq. spectrum
    hN = N//2
    # generate all sine spectral lobes
    for i in range(0, ipfreq.size):
        # it should be in range ]0,hN-1[
        loc = N * ipfreq[i] / fs
        if loc==0 or loc>hN-1: continue
        binremainder = round(loc)-loc;
        # main lobe (real value) bins to read
        lb = np.arange(binremainder-4, binremainder+5)
        # lobe magnitudes of the complex exponential
        lmag = genBhLobe(lb) * 10**(ipmag[i]/20)
        b = np.arange(round(loc)-4, round(loc)+5, dtype='int')
        for m in range(0, 9):
            # peak lobe crosses DC bin
            if b[m] < 0:
                Y[-b[m]] += lmag[m]*np.exp(-1j*ipphase[i])
            # peak lobe croses Nyquist bin
            elif b[m] > hN:
                Y[b[m]] += lmag[m]*np.exp(-1j*ipphase[i])
            # peak lobe in the limits of the spectrum
            elif b[m] == 0 or b[m] == hN:
                Y[b[m]] += lmag[m]*np.exp(1j*ipphase[i]) + lmag[m]*np.exp(-1j*ipphase[i])
            # peak lobe in positive freq. range
            else:
                Y[b[m]] += lmag[m]*np.exp(1j*ipphase[i])
        # fill the negative part of the spectrum
        Y[hN+1:] = Y[hN-1:0:-1].conjugate()
    return Y

 # analysis and synthesis by sine model
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
        mX, pX = speech_model_dft.dftAnal(x1, w, N)
        # detect locations of peaks
        ploc = peakDetection(mX, t)

        iploc, ipmag, ipphase = peakInterp(mX, pX, ploc)
        # convert peak locations to Hertz
        ipfreq = fs*iploc/float(N)
        #-----synthesis-----
        # generate sines in the spectrum
        Y = genSpecSines(ipfreq, ipmag, ipphase, Ns, fs)
        print('Y size = ' + str(Y.size))
        # compute inverse FFT
        #fftbuffer = np.real(speech_fft.ifft(Y,Ns))
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

def sineTracking(pfreq, pmag, pphase, tfreq, freqDevOffset=20, freqDevSlope=0.01):
    """
    Tracking sinusoids from one frame to the next
    pfreq, pmag, pphase: frequencies and magnitude of current frame
    tfreq: frequencies of incoming tracks from previous frame
    freqDevOffset: minimum frequency deviation at 0Hz 
    freqDevSlope: slope increase of minimum frequency deviation
    returns tfreqn, tmagn, tphasen: frequency, magnitude and phase of tracks
    """

    print('input tfreq = ')
    print(tfreq)
    tfreqn = np.zeros(tfreq.size)                              # initialize array for output frequencies
    tmagn = np.zeros(tfreq.size)                               # initialize array for output magnitudes
    tphasen = np.zeros(tfreq.size)                             # initialize array for output phases

    #找到pfreq中非0项的索引，存放在pindexes中
    pindexes = np.array(np.nonzero(pfreq), dtype=np.int)[0]    # indexes of current peaks

    #找到tfreq中非0项的索引存放在incomingTracks中
    incomingTracks = np.array(np.nonzero(tfreq), dtype=np.int)[0] # indexes of incoming tracks
    newTracks = np.zeros(tfreq.size, dtype=np.int) -1           # initialize to -1 new tracks

    #print('pmag:')
    #print(pmag)

    #按pmag中的值的升序，排列pindexes中的索引值，并将值存储在magOrder中
    magOrder = np.argsort(-pmag[pindexes])                      # order current peaks by magnitude
    print('magOrder=')
    print(magOrder)

    pfreqt = np.copy(pfreq)                                     # copy current peaks to temporary array
    pmagt = np.copy(pmag)                                       # copy current peaks to temporary array
    pphaset = np.copy(pphase)                                   # copy current peaks to temporary array

    # continue incoming tracks
    if incomingTracks.size > 0:                                 # if incoming tracks exist
        for i in magOrder:                                        # iterate over current peaks
            if incomingTracks.size == 0:                            # break when no more incoming tracks
                break
            #找到tfreq[incomingTracks](即为tfreq中索引为incomingTracks的所有项)中和pfreqt[i]最接近的项在incomingTracks中的位置（索引序号作为track返回）

            #看一下下面的例子：
            #pfreqt[i] = 14667.517927647528
            #tfreq[incomingTracks] = [  274.79298552  6023.83630708  1476.13260625  1771.88903756
            # 8363.50422752 11026.18620848  3136.9332174   5750.62764243
            #11909.6394774  11669.03168085 12426.13246761  9015.45117853
            #15535.04724514]

            #abs(pfreqt[i] - tfreq[incomingTracks]) =
            #[14392.72494213  8643.68162057 13191.3853214  12895.62889009
            #6304.01370013  3641.33171917 11530.58471024  8916.89028522
            #2757.87845025  2998.4862468   2241.38546004  5652.06674912
            #867.52931749]

            #incomingTracks=[  0   7   8  52  56  57  61  68  76  82  92 122 124]
            #则得到的track = 12，即为incomingTracks中最后一个项的索引，意思就是pfreqt[i]和tfreq[124]项的值最接近，其差的绝对值是867.52931749


            track = np.argmin(abs(pfreqt[i] - tfreq[incomingTracks]))   # closest incoming track to peak

            print('--------')
            print('when i = '+str(i))
            print('pfreqt[i] = '+str(pfreqt[i]))
            #print(i)
            #print(pfreq[i])
            print('abs(pfreqt[i] - tfreq[incomingTracks]) = ')
            print(abs(pfreqt[i] - tfreq[incomingTracks]))

            print('so we finded the track = '+str(track))
            print(tfreq[incomingTracks])
            print('incomingTracks=')
            print(incomingTracks)

            freqDistance = abs(pfreq[i] - tfreq[incomingTracks[track]]) # measure freq distance
            print('freqDistance='+str(freqDistance));
            print('--------')
            if freqDistance < (freqDevOffset + freqDevSlope * pfreq[i]):  # choose track if distance is small 
                #当前帧中的第i个peak和跟踪帧中第incomingTracks[track]个最接近
                newTracks[incomingTracks[track]] = i                      # assign peak index to track index
                #删除跟踪帧中被当前帧跟踪上的peak的索引
                incomingTracks = np.delete(incomingTracks, track)         # delete index of track in incomming tracks

                print('newTracks');
                print(newTracks)

    #找出newTracks中值不为-1（被赋值为i的，已经成功跟踪上的项）的所有项的索引
    indext = np.array(np.nonzero(newTracks != -1), dtype=np.int)[0]   # indexes of assigned tracks

    print('newTracks');
    print(newTracks)
    print('indext')
    print(indext)

    if indext.size > 0:
        #indexp是被跟踪上的输入数据中的peak的index
        indexp = newTracks[indext]                                    # indexes of assigned peaks
        print('indexp')
        print(indexp)
        print('indext')
        print(indext)
        #把输入数据pfreqt中，所有跟踪成功peak的频率，按在输入数据pfreqt中的索引顺序，放入tfreqn中
        tfreqn[indext] = pfreqt[indexp]                               # output freq tracks

        print('tfreqn[indext] = pfreqt[indexp]')
        print('tfreqn = ')
        print(tfreqn)

        #把输入数据pmagt中，所有跟踪成功peak的幅值大小，按在输入数据pmagt中的索引顺序，放入pmagt中
        tmagn[indext] = pmagt[indexp]                                 # output mag tracks
        print('tmagn[indext] = pmagt[indexp]')
        print(tmagn)
        #同上pmagt
        tphasen[indext] = pphaset[indexp]                             # output phase tracks 

        #删除输入被跟踪上的数据，下同
        pfreqt= np.delete(pfreqt, indexp)                             # delete used peaks
        print('pfreqt= np.delete(pfreqt, indexp)')

        print('pfreqt=')
        print(pfreqt)

        pmagt= np.delete(pmagt, indexp)                               # delete used peaks
        print('pmagt= np.delete(pmagt, indexp)')
        print('pmagt=')
        print(pmagt)

        pphaset= np.delete(pphaset, indexp)                           # delete used peaks

    # create new tracks from non used peaks
    #获得跟踪频率为空值0的索引
    emptyt = np.array(np.nonzero(tfreq == 0), dtype=np.int)[0]      # indexes of empty incoming tracks

    print('emptyt=')
    print(emptyt)
    #将没有跟踪上的数据，按pmagt的值，从小到大排序，并把排序后的序号存放在peaksleft中
    peaksleft = np.argsort(-pmagt)                                  # sort left peaks by magnitude
    print('peaksleft=')
    print(peaksleft)


    if ((peaksleft.size > 0) & (emptyt.size >= peaksleft.size)):    # fill empty tracks

        #如果跟踪数据中有空值，并且空值数目比剩余的没有跟踪上的输入频率数目大
        #则把跟踪频率中的空（值为0）位置的tfreqn，用输入剩余数据填充，没填满的数据都为0，因为 tfreqn初始值为0.
        tfreqn[emptyt[:peaksleft.size]] = pfreqt[peaksleft]
        tmagn[emptyt[:peaksleft.size]] = pmagt[peaksleft]
        tphasen[emptyt[:peaksleft.size]] = pphaset[peaksleft]
        print('case if')
    elif ((peaksleft.size > 0) & (emptyt.size < peaksleft.size)):   # add more tracks if necessary
        #如果空值数目比剩余的没有跟踪上的输入频率数目小
        #用输入剩余数据填充tfreqn，共填充emptyt.size个数据。
        tfreqn[emptyt] = pfreqt[peaksleft[:emptyt.size]] 
        tmagn[emptyt] = pmagt[peaksleft[:emptyt.size]]
        tphasen[emptyt] = pphaset[peaksleft[:emptyt.size]]
        #将输入的数据减去emptyt.size个上一步已经填充了的数据，再剩余的部分，扩展tfreqn的容量，并添加到tfreqn的扩展部分
        tfreqn = np.append(tfreqn, pfreqt[peaksleft[emptyt.size:]])
        tmagn = np.append(tmagn, pmagt[peaksleft[emptyt.size:]])
        tphasen = np.append(tphasen, pphaset[peaksleft[emptyt.size:]])
        print('case elif')

    print('tfreqn siz = '+str(tfreqn.size))
    print(tfreqn)

    print('tmagn=')
    print(tmagn)

    print('tphasen=')
    print(tphasen)
    return tfreqn, tmagn, tphasen

def cleaningSineTracks(tfreq, minTrackLength=3):
    """
    Delete short fragments of a collection of sinusoidal tracks 
    tfreq: frequency of tracks
    minTrackLength: minimum duration of tracks in number of frames
    returns tfreqn: output frequency of tracks
    """

    if tfreq.shape[1] == 0:                                 # if no tracks return input
            return tfreq
    nFrames = tfreq[:,0].size                               # number of frames
    nTracks = tfreq[0,:].size                               # number of tracks in a frame
    for t in range(nTracks):                                # iterate over all tracks
            trackFreqs = tfreq[:,t]                               # frequencies of one track
            trackBegs = np.nonzero((trackFreqs[:nFrames-1] <= 0)  # begining of track contours
                                                            & (trackFreqs[1:]>0))[0] + 1
            if trackFreqs[0]>0:
                    trackBegs = np.insert(trackBegs, 0, 0)
            trackEnds = np.nonzero((trackFreqs[:nFrames-1] > 0)   # end of track contours
                                                            & (trackFreqs[1:] <=0))[0] + 1
            if trackFreqs[nFrames-1]>0:
                    trackEnds = np.append(trackEnds, nFrames-1)
            trackLengths = 1 + trackEnds - trackBegs              # lengths of trach contours
            for i,j in zip(trackBegs, trackLengths):              # delete short track contours
                    if j <= minTrackLength:
                            trackFreqs[i:i+j] = 0
    return tfreq

    
    
def sineModelAnal(x, fs, w, N, H, t, maxnSines = 100, minSineDur=.01, freqDevOffset=20, freqDevSlope=0.01):
    """
    Analysis of a sound using the sinusoidal model with sine tracking
    x: input array sound, w: analysis window, N: size of complex spectrum, H: hop-size, t: threshold in negative dB
    maxnSines: maximum number of sines per frame, minSineDur: minimum duration of sines in seconds
    freqDevOffset: minimum frequency deviation at 0Hz, freqDevSlope: slope increase of minimum frequency deviation
    returns xtfreq, xtmag, xtphase: frequencies, magnitudes and phases of sinusoidal tracks
    """

    # raise error if minSineDur is smaller than 0
    if (minSineDur <0):
        raise ValueError("Minimum duration of sine tracks smaller than 0")

    print('w.size = '+str(w.size))

    hM1 = int(math.floor((w.size+1)/2))                     # half analysis window size by rounding
    hM2 = int(math.floor(w.size/2))                         # half analysis window size by floor

    print('hM1 = '+str(hM1))
    print('hM2 = '+str(hM2))
    print('hM1 + hM2 = '+str(hM2+hM1))

    x = np.append(np.zeros(hM2),x)                          # add zeros at beginning to center first window at sample 0
    x = np.append(x,np.zeros(hM2))                          # add zeros at the end to analyze last sample
    pin = hM1                                               # initialize sound pointer in middle of analysis window       
    pend = x.size - hM1                                     # last sample to start a frame
    w = w / sum(w)                                          # normalize analysis window
    tfreq = np.array([])
    # while input sound pointer is within sound
    print('sineModelAnal:pin ='+str(pin)+',pend = '+str(pend))
    while pin<pend:
        # select frame
        x1 = x[pin-hM1:pin+hM2]
        # compute dft
        mX, pX = speech_model_dft.dftAnal(x1, w, N)
        # detect locations of peaks
        ploc = peakDetection(mX, t)
        # refine peak values by interpolation
        iploc, ipmag, ipphase = peakInterp(mX, pX, ploc)
        print('sineModelAnal,mX.size = '+ str(mX.size))
        print('sineModelAnal,iploc.size = '+ str(iploc.size))
        print('sineModelAnal,iploc = ')
        print(iploc)
        print('sineModelAnal,ipmag = ')
        print(ipmag)

        # convert peak locations to Hertz
        ipfreq = fs*iploc/float(N)
        print('sineModelAnal,ipfreq = ')
        print(ipfreq)

        # perform sinusoidal tracking by adding peaks to trajectories
        tfreq, tmag, tphase = sineTracking(ipfreq, ipmag, ipphase, tfreq, freqDevOffset, freqDevSlope)
        # limit number of tracks to maxnSines
        tfreq = np.resize(tfreq, min(maxnSines, tfreq.size))
        # limit number of tracks to maxnSines
        tmag = np.resize(tmag, min(maxnSines, tmag.size))
        # limit number of tracks to maxnSines
        tphase = np.resize(tphase, min(maxnSines, tphase.size))
        # temporary output array
        jtfreq = np.zeros(maxnSines)
        # temporary output array
        jtmag = np.zeros(maxnSines)
        # temporary output array
        jtphase = np.zeros(maxnSines)
        # save track frequencies to temporary array
        jtfreq[:tfreq.size]=tfreq
        # save track magnitudes to temporary array
        jtmag[:tmag.size]=tmag
        # save track magnitudes to temporary array
        jtphase[:tphase.size]=tphase
        # if first frame initialize output sine tracks
        if pin == hM1:
            xtfreq = jtfreq 
            xtmag = jtmag
            xtphase = jtphase
        # rest of frames append values to sine tracks
        else:
            xtfreq = np.vstack((xtfreq, jtfreq))
            xtmag = np.vstack((xtmag, jtmag))
            xtphase = np.vstack((xtphase, jtphase))
        pin += H
    print('sineModelAnal:pin ='+str(pin)+',pend = '+str(pend))
    # delete sine tracks shorter than minSineDur
    xtfreq = cleaningSineTracks(xtfreq, round(fs*minSineDur/H))  
    return xtfreq, xtmag, xtphase

def test_Interp():
    xp=[1,2,3]
    fp=[3,2,0]
    x=[0,1,1.57,2.5,3.14]
    ix=np.interp(x,xp,fp)
    ix2=Interp(x,xp,fp)
    print(ix)
    print(ix2)

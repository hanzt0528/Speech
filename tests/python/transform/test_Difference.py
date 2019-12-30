import pdb
import numpy as np 
import matplotlib.pyplot as plt
from ispeech.source import *
from ispeech.transform import *

def Difference2(signal):
    a = np.zeros(signal.size)
    for k in np.arange(signal.size):
        for i in np.arange(signal.size-k):
            a[k] += (signal[i]-signal[i+k])*(signal[i]-signal[i+k])

    return a

def Difference(signal):
    yinBufferSize = int(signal.size/2)
    a = np.zeros(signal.size,dtype=float)
    for i in range(1,yinBufferSize):
        startPoint = yinBufferSize/2-i/2
        endPoint = startPoint + yinBufferSize
        for j in range(int(startPoint),int(endPoint)):
            delta = signal[i+j]-signal[j]
            a[i] += delta*delta

    return a
def cumulativeDifference(a,yinBufferSize):
    
    a[0]=1
    suming = 0.0
    for tau in np.arange(1,int(yinBufferSize)):
        suming+=a[tau]
        if suming == 0:
            a[tau] = 1
        else:
            a[tau]*=tau/suming
    return a

def absoluteThreshold(yinBuffer,thresh):
    tau = 0
    minTau = 0
    minVal = 1000
    
    # using Joren Six's "loop construct" from TarsosDSP
    tau = 2

    yinBufferSize = yinBuffer.size
    while tau < yinBufferSize :
        if yinBuffer[tau] < thresh :
            while tau+1 < yinBufferSize and yinBuffer[tau+1] < yinBuffer[tau]:
                tau+=1
            return tau
        else:
            if yinBuffer[tau] < minVal:
                minVal = yinBuffer[tau]
                minTau = tau;
        tau+=1
        #print(tau)
    if minTau > 0 :
        return -minTau
    return 0
betaDist2 = [0.012614,0.022715,0.030646,0.036712,0.041184,0.044301,0.046277,0.047298,0.047528,0.047110,0.046171,0.044817,0.043144,0.041231,0.039147,0.036950,0.034690,0.032406,0.030133,0.027898,0.025722,0.023624,0.021614,0.019704,0.017900,0.016205,0.014621,0.013148,0.011785,0.010530,0.009377,0.008324,0.007366,0.006497,0.005712,0.005005,0.004372,0.003806,0.003302,0.002855,0.002460,0.002112,0.001806,0.001539,0.001307,0.001105,0.000931,0.000781,0.000652,0.000542,0.000449,0.000370,0.000303,0.000247,0.000201,0.000162,0.000130,0.000104,0.000082,0.000065,0.000051,0.000039,0.000030,0.000023,0.000018,0.000013,0.000010,0.000007,0.000005,0.000004,0.000003,0.000002,0.000001,0.000001,0.000001,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000];
betaDist4 = [0.003996,0.007596,0.010824,0.013703,0.016255,0.018501,0.020460,0.022153,0.023597,0.024809,0.025807,0.026607,0.027223,0.027671,0.027963,0.028114,0.028135,0.028038,0.027834,0.027535,0.027149,0.026687,0.026157,0.025567,0.024926,0.024240,0.023517,0.022763,0.021983,0.021184,0.020371,0.019548,0.018719,0.017890,0.017062,0.016241,0.015428,0.014627,0.013839,0.013068,0.012315,0.011582,0.010870,0.010181,0.009515,0.008874,0.008258,0.007668,0.007103,0.006565,0.006053,0.005567,0.005107,0.004673,0.004264,0.003880,0.003521,0.003185,0.002872,0.002581,0.002312,0.002064,0.001835,0.001626,0.001434,0.001260,0.001102,0.000959,0.000830,0.000715,0.000612,0.000521,0.000440,0.000369,0.000308,0.000254,0.000208,0.000169,0.000136,0.000108,0.000084,0.000065,0.000050,0.000037,0.000027,0.000019,0.000014,0.000009,0.000006,0.000004,0.000002,0.000001,0.000001,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,0.000000];

def yinProb(yinBuffer,prior,yinBufferSize,minTau0 = 0,maxTau0 = 0):
    minTau = 2
    maxTau = yinBufferSize
    if minTau0 >0 and minTau0 < maxTau0 :
        minTau = minTau0
    if maxTau0 >0 and maxTau0 < yinBufferSize and maxTau0 >minTau :
        maxTau = maxTau0
        
    minWeigth = 0.01

    nThreshold = 100    
    nThresholdInt = nThreshold
    
    distribution = np.zeros(nThresholdInt,dtype=float)
    thresholds = np.zeros(nThresholdInt,dtype=float)
    peakProb = np.zeros(int(yinBufferSize),dtype=float)



    for i in np.arange(nThresholdInt):
        distribution[i] = betaDist2[i]
        #distribution[i] = 0.001
        thresholds[i]=0.01+i*0.01


    currThreshInd = nThreshold -1
    tau = minTau
    minInd = 0
    minVal =42.0

    sumProb = 0.0

    while tau+1 < maxTau:
        if yinBuffer[tau] < thresholds[nThresholdInt-1] and yinBuffer[tau+1] < yinBuffer[tau]:
            while (tau+1) < maxTau and yinBuffer[tau+1]<yinBuffer[tau]:
                tau+=1

            if yinBuffer[tau]<minVal and tau>2 :
                minVal=yinBuffer[tau]
                minInd = tau

            currThreshInd = nThresholdInt -1

            """
            while thresholds[currThreshInd] > yinBuffer[tau] and currThreshInd >-1 :
                peakProb[tau]+=distribution[currThreshInd];
                currThreshInd = currThreshInd-1
            """

            print('pos = '+str(tau)+' value='+str(yinBuffer[tau]))
            while thresholds[currThreshInd] > yinBuffer[tau] and currThreshInd >-1 :
                peakProb[tau]+=distribution[currThreshInd];
                currThreshInd = currThreshInd-1
                
            sumProb +=peakProb[tau]
            tau+=1
        else:
            tau+=1

    print('minVal='+str(minVal))
    print('minInd='+str(minInd))
    print('peakProb[minInd] = ' + str(peakProb[minInd]))
    if peakProb[minInd] > 1 :
        return np.zeros(yinBufferSize,dtype=float)

    nonPeakProb = 1.0
    print('sumProb = '+str(sumProb))
    if sumProb > 0 :
        for i in range(minTau,maxTau):
            peakProb[i]=peakProb[i]/sumProb*peakProb[minInd]
            peakProb[i]=peakProb[i]/sumProb
            nonPeakProb  -= peakProb[i]

    print('sum of prob = '+ str(sum(peakProb)))
    #if minInd > 0 :
        #peakProb[minInd]+=nonPeakProb*minWeigth

    return peakProb
        
    
        
def DetectPitch(a,fs):


    kmax =absoluteThreshold(a,0.1)

    aperiodicity = a[abs(kmax)]
    periodicity = 1-aperiodicity
    print('periodicity = '+ str(periodicity))

    
    """
    kmax = 0
    for k in np.arange(1,a.size-1):
        if a[k] < 0.002 :
            kmax = k
            break
    """
    kmax = peakInterp(a,kmax)
    return kmax,fs/kmax

def peakInterp(mX,ploc):
    """
    Interpolate peak values using parabolic interpolation
    mX, pX: magnitude and phase spectrum, ploc: locations of peaks
    returns iploc, ipmag, ipphase: interpolated peak location, magnitude and phase values
    """

    # magnitude of peak bin
    val = mX[ploc]
    # magnitude of bin at left
    lval = mX[ploc-1]
    # magnitude of bin at right
    rval = mX[ploc+1]
    # center of parabola
    iploc = ploc + 0.5*(lval-rval)/(lval-2*val+rval)

    return iploc

def test():   
    signal = [1,1,1,1,-1,-1,-1,-1,1,1,1,1,-1,-1,-1,-1];

    signal = np.array(signal)

    fs = 11025
    t =   np.arange(0,0.2,1.0/fs)
    
    signal = np.cos(2*np.pi*100*t)+np.cos(2*np.pi*200*t)+np.cos(2*np.pi*400*t)

    plt.figure(1, figsize=(9.5, 6))

    plt.subplot(3,1,1)
    plt.plot(np.arange(signal.size),signal,label="signal")

    plt.title("signal = np.cos(2*np.pi*100*t)+np.cos(2*np.pi*200*t)+np.cos(2*np.pi*400*t)")
    a = Difference(signal)
    a = cumulativeDifference(a,int(signal.size/2))

    peakProbability = yinProb(a,2,int(signal.size/2),0,0)

    for iBuf in np.arange(peakProbability.size):
        if peakProbability[iBuf] > 0 :
            new_pos=peakInterp(a,iBuf)
            currentF0 = fs * (1.0 / new_pos)
            #print('frequency '+ str(currentF0) + 'prob:'+str(peakProbability[iBuf]))
            print('pos '+ str(iBuf) + '  prob:'+str(peakProbability[iBuf])) 

    #print('peakProbability=')
    #print(peakProbability)

    
    kmax,f = DetectPitch(a,fs)

    print('kmax = ' + str(kmax))
    print('f = '+ str(fs/kmax))
    plt.subplot(3,1,2)

    plt.plot(np.arange(a.size),a,label="when t=0.2s " + "kmax =" + str(kmax)+ " f="+ str(f));

    t =   np.arange(0,0.06,1.0/fs)
    
    signal = np.cos(2*np.pi*100*t)+np.cos(2*np.pi*200*t)+np.cos(2*np.pi*400*t)

    a = Difference(signal)
    a = cumulativeDifference(a,signal.size/2)
    kmax,f = DetectPitch(a,fs)

    print('kmax = ' + str(kmax))
    print('f = '+ str(fs/kmax))

    plt.plot(np.arange(a.size),a,label="when t=0.02s " + "kmax =" + str(kmax)+ " f="+ str(f));
    plt.title("Autocorrelation of signal")
    plt.legend()
    plt.subplot(3,1,3)
    plt.plot(np.arange(len(betaDist4)),betaDist4)
    plt.legend()
    plt.show()
    
   
test()

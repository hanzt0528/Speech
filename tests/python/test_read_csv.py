import numpy as np
from matplotlib import pyplot as plt
import csv

f=open("/Users/hanzhongtao/Programs/Speech/data/midi/twinkle.f0.csv")
f_csv=csv.reader(f)

time = []
freq = []
for row in f_csv:
    if row[0] == "time":
        continue
    time.append(float(row[0]))
    freq.append(float(row[1]))

time = np.array(time[1:])
freq = np.array(freq[1:])
plt.plot(np.arange(freq.size),freq)
plt.show()

    

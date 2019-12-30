
import numpy as np
import matplotlib.pyplot as plt
a = np.arange(10)

print(a)

print('-------')
print(a[-2:0:-1])

for i in range(0,100):
    ran = np.random.rand(10)
    plt.plot(np.arange(10),ran,marker = 'x')

plt.show()

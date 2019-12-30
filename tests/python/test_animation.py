import numpy as np
from matplotlib import pyplot as plt
from matplotlib.animation import FuncAnimation
import matplotlib.patheffects as path_effects
plt.style.use('seaborn-pastel')
  
fig = plt.figure()


ax = plt.axes(xlim=(0, 4), ylim=(-2, 2))


text = ax.text(0.5, 0.5, [], color='white',
                          ha='center', va='center', size=30)

text.set_path_effects([path_effects.Stroke(linewidth=3, foreground='black'),
                       path_effects.Normal()])

line, = ax.plot([], [], lw=3)
 
def init():
    #line.set_data([], [])

    return line,

def animate(i):
    x = np.linspace(0, 4, 1000)
    y = np.sin(2 * np.pi * (x - 0.01 * i))

    line.set_data(x, y)

    return line,
 
anim = FuncAnimation(fig, animate, init_func=init,
                               frames=100, interval=20, blit=True)

plt.show()

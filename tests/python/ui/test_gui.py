import sys
import matplotlib
matplotlib.use('TkAgg')
try:
    # for Python2
    from Tkinter import *   ## notice capitalized T in Tkinter 
except ImportError:
    # for Python3
    from tkinter import *   ## notice lowercase 't' in tkinter here

from notebook import *
from gui_frame import *

root = Tk()

root.title("gui test")

nb = notebook(root,TOP)
f1 = Frame(nb())
dft = DftModel_frame(f1)

nb.add_screen(f1,"DFT")

nb.display(f1)

root.geometry('+0+0')

root.mainloop()


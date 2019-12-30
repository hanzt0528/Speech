
import ctypes
os = ctypes.cdll.LoadLibrary
lib=os('./libpycallclass.so')
#lib = ctypes.cdll.LoadLibraty(".\libpycallclass.so")
print('display')
lib.display()

print('display(100)')
lib.display_int(100)

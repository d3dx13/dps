import ctypes
import os

# import pwnlib.shellcraft as sc

# os.pidfd_open()
# os.pidfd_getfd()

libc = ctypes.CDLL(None)
syscall = libc.syscall

print(syscall(17))
print(dir(os))

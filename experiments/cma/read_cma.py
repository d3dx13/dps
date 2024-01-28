import time
from time import sleep
from dma_heap import *
import mmap
import numpy as np
import devmem
import os, errno
import socket

print(os.getpid())


def silentremove(filename):
    try:
        os.remove(filename)
    except OSError as e:  # this would be "except OSError, e:" before Python 2.6
        if e.errno != errno.ENOENT:  # errno.ENOENT = no such file or directory
            raise  # re-raise exception if a different error occurred


# dmaHeap = DmaHeap()

path = "/dev/shm/sender_fd"
sender = socket.socket(socket.AF_UNIX)
silentremove(path)
sender.bind(path)
sender.listen()

sock, addr = sender.accept()
print("sock, addr", sock, addr)

tup = socket.recv_fds(sock, 1024, 2)
fdList = tup[1]
print("fdList", fdList)

frame_size = 1024 * 1024 * 128
memory = mmap.mmap(fdList[0], frame_size, mmap.MAP_SHARED, mmap.PROT_READ | mmap.PROT_WRITE)
arr = np.ndarray(shape=(1024, 1024, 128), dtype=np.uint8, buffer=memory)
local_arr = np.ndarray(shape=(1024, 1024, 128), dtype=np.uint8)

while True:
    t_start = time.monotonic_ns()
    local_arr[:] = arr[:]
    t_end = time.monotonic_ns()
    freq = np.prod(arr.shape) * arr.dtype.itemsize / (t_end - t_start)

    print()
    print(np.mean(arr))
    print(f" freq = {freq:.6f} Gb sub / sec")

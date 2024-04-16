import time
from time import sleep
import cv2
from dma_heap import *
import mmap
import numpy as np
import devmem
import os, errno
import socket
import sys
sys.path.append("../cpp_python/build/lib.linux-aarch64-cpython-311")

import dma_heap
import dma_cma

print(os.getpid())


def silentremove(filename):
    try:
        os.remove(filename)
    except OSError as e:  # this would be "except OSError, e:" before Python 2.6
        if e.errno != errno.ENOENT:  # errno.ENOENT = no such file or directory
            raise  # re-raise exception if a different error occurred

dmaHeap = dma_heap.DmaHeap()
pub_pid = 2813
pub_fd = 44
target_fd = dma_cma.borrow_fd_from_pid(pub_pid, pub_fd)

frame_size = 320 * 240 * 3
memory = mmap.mmap(target_fd, frame_size, mmap.MAP_SHARED, mmap.PROT_READ | mmap.PROT_WRITE)
arr = np.ndarray(shape=(320 * 240 * 3), dtype=np.uint8, buffer=memory)
local_arr = np.ndarray(shape=(320 * 240 * 3), dtype=np.uint8)

import matplotlib
import matplotlib.pyplot
while True:
    t_start = time.monotonic_ns()
    local_arr[:] = arr[:]
    t_end = time.monotonic_ns()
    freq = np.prod(arr.shape) * arr.dtype.itemsize / (t_end - t_start)
    print()
    print(np.mean(arr))
    print(f" freq = {freq:.6f} Gb sub / sec")
    
    print(arr)
    matplotlib.pyplot.imshow(local_arr)


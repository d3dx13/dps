import time
from time import sleep
from dma_heap import *
import mmap
import numpy as np
import devmem
import os, errno
import socket
import sys
sys.path.append("../cpp_python/build/lib.linux-aarch64-cpython-311")

import answer

print(os.getpid())


def silentremove(filename):
    try:
        os.remove(filename)
    except OSError as e:  # this would be "except OSError, e:" before Python 2.6
        if e.errno != errno.ENOENT:  # errno.ENOENT = no such file or directory
            raise  # re-raise exception if a different error occurred


# dmaHeap = DmaHeap()
pub_pid = 3281
pub_fd = 4
target_fd = answer.get_answer()

frame_size = 1024 * 1024 * 512
memory = mmap.mmap(target_fd, frame_size, mmap.MAP_SHARED, mmap.PROT_READ | mmap.PROT_WRITE)
arr = np.ndarray(shape=(1024, 1024, 512), dtype=np.uint8, buffer=memory)
local_arr = np.ndarray(shape=(1024, 1024, 512), dtype=np.uint8)

while True:
    t_start = time.monotonic_ns()
    local_arr[:] = arr[:]
    t_end = time.monotonic_ns()
    freq = np.prod(arr.shape) * arr.dtype.itemsize / (t_end - t_start)

    print()
    print(np.mean(arr))
    print(f" freq = {freq:.6f} Gb sub / sec")

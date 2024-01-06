import time
from time import sleep
from dma_heap import *
import mmap
import numpy as np

dmaHeap = DmaHeap()

frame_size = 1024 * 1024 * 1024

fd_connected = dmaHeap.connect(22071, 4)
memory_connected = mmap.mmap(fd_connected.get(), frame_size, mmap.MAP_SHARED, mmap.PROT_READ | mmap.PROT_WRITE)
arr_connected = np.ndarray(shape=(1024, 1024, 1024), dtype=np.uint8, buffer=memory_connected)

rand_arr = np.random.randint(0, 255, size=(1024, 1024, 1024), dtype=np.uint8)
while True:
    to = time.monotonic_ns()
    rand_arr[:] = arr_connected[:]
    print()
    print(frame_size / (time.monotonic_ns() - to))
    print(memory_connected[:100])

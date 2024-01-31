import time
from time import sleep
import os
from dma_heap import *
import mmap
import numpy as np
import ctypes
import psutil
import zmq
import array
import os, errno
import socket

print("\nStart")
os.system("cat /proc/meminfo | grep Cma")

frame_size = 1024 * 1024 * 128

dmaHeap = DmaHeap()

fd = dmaHeap.alloc(f"test", frame_size)

print("\nAlloc")
os.system("cat /proc/meminfo | grep Cma")

memory = mmap.mmap(fd.get(), frame_size, mmap.MAP_SHARED, mmap.PROT_READ | mmap.PROT_WRITE)

print("\nmmap")
os.system("cat /proc/meminfo | grep Cma")

arr = np.ndarray(shape=(1024, 1024, 128), dtype=np.uint8, buffer=memory)

print("\nndarray")
os.system("cat /proc/meminfo | grep Cma")

print("dmaHeap.dmaHeap_fd", dmaHeap.dmaHeap_fd)
os.close(dmaHeap.dmaHeap_fd)

print("\nclose")
os.system("cat /proc/meminfo | grep Cma")

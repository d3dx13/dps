import sys

sys.path.append("/home/d3dx13/workspace/ips/experiments/linux_proxy/build/lib.linux-x86_64-cpython-310")

import linux_proxy
import os
from time import sleep
from dma_heap import *
import mmap
import numpy as np
import datetime

print("\nStart")
os.system("cat /proc/meminfo | grep Cma")

frame_size = 1024 * 1024 * 128

print("PID:", os.getpid())

dmaHeap = DmaHeap()

fd = dmaHeap.alloc(f"testing_name", frame_size)
print("fd", fd)

print("\nAlloc")
os.system("cat /proc/meminfo | grep Cma")

memory = mmap.mmap(fd, frame_size, mmap.MAP_SHARED, mmap.PROT_READ | mmap.PROT_WRITE)

print("\nmmap")
os.system("cat /proc/meminfo | grep Cma")

while True:
    date_time = datetime.datetime.now().strftime("%m/%d/%Y, %H:%M:%S").encode()
    print(date_time)
    memory[:len(date_time)] = date_time
    sleep(1.0)

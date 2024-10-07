import sys

sys.path.append("/home/d3dx13/workspace/ips/experiments/dma_proxy/build/lib.linux-x86_64-3.10")
sys.path.append("/home/d3dx13/workspace/ips/experiments/dma_proxy/build/lib.linux-aarch64-cpython-311")
sys.path.append("/home/d3dx13/workspace/ips/experiments/dma_proxy/build/lib.linux-x86_64-cpython-310")

import os
from time import sleep
import dma_proxy
import mmap
import numpy as np
import datetime
import select
import cv2
import posix_ipc

import resource
print(resource.getrlimit(resource.RLIMIT_NOFILE))
print(resource.setrlimit(resource.RLIMIT_NOFILE, (1048576, 1048576)))

BUFF_LENGTH = 63000
# BUFF_SIZE = 1024 ** 3 * 114 // BUFF_LENGTH
BUFF_SIZE = 4096

print(f"Size: {BUFF_SIZE * BUFF_LENGTH / 1024}kB")

print("\n Start")
os.system("cat /proc/meminfo | grep MemAvailable")
print("PID:", os.getpid())

dmabuf_heap_fd = dma_proxy.dmabuf_heap_open()
print("dmabuf_heap_fd", dmabuf_heap_fd)

print("\n Allocated")
dmabuf_fds = []
memory = []
for i in range(BUFF_LENGTH):
    dmabuf_fds.append(dma_proxy.dmabuf_heap_alloc(dmabuf_heap_fd, f"test_{i}", BUFF_SIZE))
    memory.append(mmap.mmap(dmabuf_fds[i], BUFF_SIZE, mmap.MAP_SHARED, mmap.PROT_READ | mmap.PROT_WRITE))
os.system("cat /proc/meminfo | grep MemAvailable")
print("dmabuf_fds", len(dmabuf_fds))

for i in range(BUFF_LENGTH):
    dma_proxy.close(dmabuf_fds[i])
    memory[i].close()
dma_proxy.close(dmabuf_heap_fd)

print("\n Closed")
os.system("cat /proc/meminfo | grep MemAvailable")
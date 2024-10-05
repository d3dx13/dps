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

import resource
print(resource.getrlimit(resource.RLIMIT_NOFILE))
print(resource.setrlimit(resource.RLIMIT_NOFILE, (1048576, 1048576)))

BUFF_SIZE = 640 * 480 * 3
BUFF_LENGTH = 10

os.system("cat /proc/meminfo | grep Cma")
print("PID:", os.getpid())

dmabuf_heap_fd = dma_proxy.dmabuf_heap_open()
print("dmabuf_heap_fd", dmabuf_heap_fd)

dmabuf_fds = []
memory = []
for i in range(BUFF_LENGTH):
    dmabuf_fds.append(dma_proxy.dmabuf_heap_alloc(dmabuf_heap_fd, f"test_{i}", BUFF_SIZE))
    memory.append(mmap.mmap(dmabuf_fds[i], BUFF_SIZE, mmap.MAP_SHARED, mmap.PROT_READ | mmap.PROT_WRITE))
os.system("cat /proc/meminfo | grep Cma")
print("dmabuf_fds", dmabuf_fds)

cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)


counter = 0
while counter < 3 * 2000:
    for i in range(BUFF_LENGTH):
        r, frame = cap.read()
        dma_proxy.dmabuf_sync_start(dmabuf_fds[i])
        memory[i][:] = frame.tobytes()
        dma_proxy.dmabuf_sync_stop(dmabuf_fds[i])
    counter += 1

cap.release()
for i in range(BUFF_LENGTH):
    dma_proxy.close(dmabuf_fds[i])
dma_proxy.close(dmabuf_heap_fd)
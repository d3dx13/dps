import sys

sys.path.append("/home/d3dx13/workspace/ips/experiments/dma_proxy/build/lib.linux-x86_64-3.10")
sys.path.append("/home/d3dx13/workspace/ips/experiments/dma_proxy/build/lib.linux-aarch64-cpython-311")
sys.path.append("/home/d3dx13/workspace/ips/experiments/dma_proxy/build/lib.linux-x86_64-cpython-310")

import os
from time import sleep, monotonic
import dma_proxy
import mmap
import datetime
import select
import numpy as np
import cv2

import resource
print(resource.getrlimit(resource.RLIMIT_NOFILE))
print(resource.setrlimit(resource.RLIMIT_NOFILE, (1048576, 1048576)))

print("PID:", os.getpid())

BUFF_SIZE = 640 * 480 * 3
BUFF_LENGTH = 10

pid_input = int(input("Enter PID: "))
pid_fd = dma_proxy.pidfd_open(pid_input)
fds_input = [4, 6, 8, 10, 12, 14, 16, 18, 20, 22]

dmabuf_fds = []
memory = []
for i in range(BUFF_LENGTH):
    dmabuf_fds.append(dma_proxy.pidfd_getfd(pid_fd, fds_input[i]))
    memory.append(mmap.mmap(dmabuf_fds[i], BUFF_SIZE, mmap.MAP_SHARED, mmap.PROT_READ | mmap.PROT_WRITE))
print("dmabuf_fds", dmabuf_fds)


i = 0
while True:
    # dma_proxy.dmabuf_sync_start(dmabuf_fds[i])
    frame = np.frombuffer(memory[i], np.uint8)
    frame = np.reshape(frame, (480, 640, 3))
    cv2.imshow("frame", frame)
    cv2.waitKey(10)
    # dma_proxy.dmabuf_sync_stop(dmabuf_fds[i])
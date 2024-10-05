import sys

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

print("PID:", os.getpid())

BUFF_SIZE = 640 * 480 * 2
BUFF_LENGTH = 2

pid_input = 38756 # int(input("Enter PID: "))
fds_input = [4,]
fds_input = [12, 13]

dmabuf_fds = []
memory = []
for i in range(BUFF_LENGTH):
    dmabuf_fds.append(dma_proxy.borrow_fd_from_pid(pid_input, fds_input[i]))
    memory.append(mmap.mmap(dmabuf_fds[i], BUFF_SIZE, mmap.MAP_SHARED, mmap.PROT_READ | mmap.PROT_WRITE))
print("dmabuf_fds", dmabuf_fds)


while True:
    frame = np.frombuffer(memory[0], np.uint8)
    frame = np.reshape(frame, (480, 640, 2))[:,:,0]
    cv2.imshow("frame1", frame)
    frame = np.frombuffer(memory[1], np.uint8)
    frame = np.reshape(frame, (480, 640, 2))[:,:,0]
    cv2.imshow("frame2", frame)
    cv2.waitKey(1)
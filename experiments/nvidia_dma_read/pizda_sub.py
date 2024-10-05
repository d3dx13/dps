import sys

sys.path.append("/home/d3dx13/workspace/ips/experiments/linux_proxy/build/lib.linux-aarch64-cpython-311")
sys.path.append("/home/d3dx13/workspace/ips/experiments/linux_proxy/build/lib.linux-x86_64-cpython-310")

import os
from time import sleep, monotonic
import linux_proxy
import mmap
import numpy as np
import datetime
import select
import cv2

print("PID:", os.getpid())

BUFF_SIZE = 1920 * 1080 * 4

pid_input = 1580 # int(input("Enter PID: "))
fds_input = [23, 25, 26, 27]
fds_input = [27, ]

dmabuf_fds = []
dmabuf_maps = {}
for i in range(len(fds_input)):
    dmabuf_fds.append(linux_proxy.borrow_fd_from_pid(pid_input, fds_input[i]))
    print("fs", dmabuf_fds[i])
    dmabuf_maps[dmabuf_fds[i]] = mmap.mmap(fileno=dmabuf_fds[i], length=BUFF_SIZE, flags=mmap.PROT_READ | mmap.PROT_WRITE, prot=mmap.MAP_SHARED)
print("dmabuf_fds", dmabuf_fds)
print("dmabuf_maps", dmabuf_maps)

event = select.epoll(sizehint=-1, flags=0)
for fd in dmabuf_fds:
    event.register(fd=fd, eventmask=select.EPOLLIN | select.EPOLLET) # | select.EPOLLET
events = set()

while True:
    res = event.poll()
    events.add(tuple(res))
    print()
    print(monotonic(), res, events)
    # print(dmabuf_maps[event[0]])
    # sleep(1.0)
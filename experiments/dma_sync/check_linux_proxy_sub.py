import sys

sys.path.append("/home/d3dx13/workspace/ips/experiments/dma_proxy/build/lib.linux-aarch64-cpython-311")
sys.path.append("/home/d3dx13/workspace/ips/experiments/dma_proxy/build/lib.linux-x86_64-cpython-310")

import os
from time import sleep, monotonic
import dma_proxy
import mmap
import numpy as np
import datetime
import select

print("PID:", os.getpid())

pid_input = 11095 # int(input("Enter PID: "))
fds_input = [4, 7, 10, 13, 16, 19, 22, 25, 28, 31]
export_input = [5, 8, 11, 14, 17, 20, 23, 26, 29, 32]

dmabuf_fds = []
dmabuf_import = []
for i in range(10):
    dmabuf_fds.append(dma_proxy.borrow_fd_from_pid(pid_input, fds_input[i]))
    dmabuf_import.append(dma_proxy.dmabuf_import_sync_file(dmabuf_fds[i], dma_proxy.borrow_fd_from_pid(pid_input, export_input[i])))
print("dmabuf_fds", dmabuf_fds)
print("dmabuf_import", dmabuf_import)


event = select.epoll(sizehint=-1, flags=0)
for fd in dmabuf_import:
    event.register(fd=fd, eventmask=select.EPOLLOUT) # | select.EPOLLET

events = set()

while True:
    res = event.poll()
    events.add(tuple(res))
    print(monotonic(), res, events)
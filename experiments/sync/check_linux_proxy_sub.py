import sys

sys.path.append("/home/d3dx13/workspace/ips/experiments/linux_proxy/build/lib.linux-x86_64-cpython-310")

import linux_proxy
import os
from time import sleep, monotonic
import mmap
import select
from dma_heap import *

dmaHeap = DmaHeap()

print("PID:", os.getpid())

pid_input = int(input("Enter PID: "))
fd_input = int(input("Enter fd: "))

fd = linux_proxy.borrow_fd_from_pid(pid_input, fd_input)
print("fd", fd)

dma_sync_fd = dmaHeap.connect(fd)
print("dma_sync_fd", dma_sync_fd)

"""
memory = mmap.mmap(fd, 1024 * 1024 * 128, mmap.MAP_SHARED, mmap.PROT_READ | mmap.PROT_WRITE)
"""

event = select.epoll(sizehint=-1, flags=0)
event.register(fd=dma_sync_fd, eventmask=select.EPOLLOUT) # | select.EPOLLET

events = set()

while True:
    res = event.poll()
    events.add(tuple(res))
    print(monotonic(), res, events)
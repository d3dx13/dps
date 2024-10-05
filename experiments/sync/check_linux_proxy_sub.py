import sys

sys.path.append("/home/d3dx13/workspace/ips/experiments/dma_proxy/build/lib.linux-x86_64-cpython-310")

import dma_proxy
import os
from time import sleep, monotonic
import mmap
import select
from dma_heap import *

dmaHeap = DmaHeap()
print("PID:", os.getpid())

pid_input = 196088 # int(input("Enter PID: "))
fd_input = 4 # int(input("Enter fd: "))

fd = dma_proxy.borrow_fd_from_pid(pid_input, fd_input)
print("fd", fd)


memory = mmap.mmap(fd, 1024 * 1024 * 1024, mmap.MAP_SHARED, mmap.PROT_READ | mmap.PROT_WRITE)


event = select.epoll(sizehint=-1, flags=0)
event.register(fd=fd, eventmask=select.EPOLLOUT | select.EPOLLET) # | select.EPOLLET
events = set()


while True:
    # res = event.poll()
    # events.add(tuple(res))
    print(monotonic())
    sleep(1.0)
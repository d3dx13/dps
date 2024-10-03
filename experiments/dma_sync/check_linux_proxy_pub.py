import sys

sys.path.append("/home/d3dx13/workspace/ips/experiments/dma_proxy/build/lib.linux-aarch64-cpython-311")
sys.path.append("/home/d3dx13/workspace/ips/experiments/dma_proxy/build/lib.linux-x86_64-cpython-310")

import os
from time import sleep
import dma_proxy
import mmap
import numpy as np
import datetime
import select


BUFF_SIZE = 640 * 480 * 3

os.system("cat /proc/meminfo | grep Cma")
print("PID:", os.getpid())

dmabuf_heap_fd = dma_proxy.dmabuf_heap_open();
print("dmabuf_heap_fd", dmabuf_heap_fd)

dmabuf_fds = []
dmabuf_export = []
dmabuf_maps = []
for i in range(10):
    dmabuf_fds.append(dma_proxy.dmabuf_heap_alloc(dmabuf_heap_fd, f"test_{i}", BUFF_SIZE))
    dmabuf_export.append(dma_proxy.dmabuf_export_sync_file(dmabuf_fds[i]))
    dmabuf_maps.append(mmap.mmap(fileno=dmabuf_fds[i], length=BUFF_SIZE, flags=mmap.PROT_WRITE | mmap.PROT_READ, prot=mmap.MAP_SHARED))
os.system("cat /proc/meminfo | grep Cma")
print("dmabuf_fds", dmabuf_fds)
print("dmabuf_export", dmabuf_export)
print("dmabuf_maps", dmabuf_maps)

counter = 0
while counter < 10:
    for i in range(10):
        dma_proxy.dmabuf_sync_start(dmabuf_fds[i])
        sleep(0.1)
        dma_proxy.dmabuf_sync_stop(dmabuf_fds[i])
    counter += 1
dma_proxy.dmabuf_heap_close(dmabuf_heap_fd)
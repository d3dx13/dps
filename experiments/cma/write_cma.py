import time
from time import sleep
import os
from dma_heap import *
import mmap
import numpy as np
import ctypes
import psutil
import zmq
import array
import os, errno
import socket

dmaHeap = DmaHeap()

frame_size = 1024 * 1024

fd = dmaHeap.alloc(f"test", frame_size)
memory = mmap.mmap(fd.get(), frame_size, mmap.MAP_SHARED, mmap.PROT_READ | mmap.PROT_WRITE)
arr = np.ndarray(shape=(1024, 1024), dtype=np.uint8, buffer=memory)

path = "/dev/shm/sender_fd"
sender = socket.socket(socket.AF_UNIX)


def update_cma_fd():
    try:
        sender.connect(path)
        desList = [fd.get(), ]
        socket.send_fds(sender, [msgb], desList, 2, None)
        print(dir(socket))
    except:
        pass


print(fd.get())
print(os.getpid())

while True:
    # msg = sock.recv()
    msg_a = "hi"
    msgb = bytes(msg_a, 'ascii')
    update_cma_fd()

    val = int(time.monotonic()) % 256
    rand_arr = np.random.randint(val, val + 1, size=(1024, 1024), dtype=np.uint8)
    to = time.monotonic_ns()
    arr[:] = rand_arr[:]
    # print(f"{frame_size / (time.monotonic_ns() - to):.6f} Gb pub / sec")

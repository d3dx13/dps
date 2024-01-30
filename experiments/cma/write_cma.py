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


def silentremove(filename):
    try:
        os.remove(filename)
    except OSError as e:  # this would be "except OSError, e:" before Python 2.6
        if e.errno != errno.ENOENT:  # errno.ENOENT = no such file or directory
            raise  # re-raise exception if a different error occurred


frame_size = 1024 * 1024 * 512

fd = dmaHeap.alloc(f"test", frame_size)
memory = mmap.mmap(fd.get(), frame_size, mmap.MAP_SHARED, mmap.PROT_READ | mmap.PROT_WRITE)
arr = np.ndarray(shape=(1024, 1024, 512), dtype=np.uint8, buffer=memory)

path_fd = f"/proc/{os.getpid()}/fd/{fd.get()}"
path = "/dev/shm/sender_fd"
# silentremove(path)
# os.link(src=path_fd, dst=path)
sender = socket.socket(socket.AF_UNIX)


def update_cma_fd():
    try:
        sender.connect(path)
        msg_a = "hi"
        msgb = bytes(msg_a, 'ascii')
        desList = [fd.get(), ]
        socket.send_fds(sender, [msgb], desList, 2, None)
        # print(dir(socket))
    except:
        pass


print(fd.get())
print(os.getpid())

while True:
    # msg = sock.recv()
    update_cma_fd()

    val = int(time.monotonic()) % 256
    rand_arr = np.random.randint(val, val + 1, size=(1024, 1024, 512), dtype=np.uint8)

    t_start = time.monotonic_ns()
    arr[:] = rand_arr[:]
    t_end = time.monotonic_ns()
    freq = np.prod(arr.shape) * arr.dtype.itemsize / (t_end - t_start)

    print()
    print(np.mean(arr))
    print(f" freq = {freq:.6f} Gb pub / sec")

import multiprocessing
from multiprocessing import shared_memory, sharedctypes
from dma_heap import *
import mmap
import numpy as np
import os
import time
import socket
import ctypes

SIZE = (1600, 1300, 3)


def pub_array(fd, affinity):
    os.sched_setaffinity(0, affinity)

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    data_tensor_alt = np.random.randint(0, 255, size=SIZE, dtype=np.uint8)
    memory = mmap.mmap(fd.get(), frame_size, mmap.MAP_SHARED, mmap.PROT_READ | mmap.PROT_WRITE)
    arr = np.ndarray(shape=SIZE, dtype=np.uint8, buffer=memory)

    data_tensor = np.random.randint(0, 255, size=SIZE, dtype=np.uint8)
    while True:
        # data_tensor = np.random.randint(0, 255, size=(6, 1024, 1024), dtype=np.uint8)

        t_start = time.monotonic_ns()
        arr[:] = data_tensor[:]
        # data_tensor_alt[:] = data_tensor[:]
        sock.sendto(b"", ("127.0.0.1", 5000 + affinity[0] + 1))
        t_end = time.monotonic_ns()
        freq = np.prod(data_tensor.shape) * data_tensor.dtype.itemsize / (t_end - t_start)
        print(f"{time.monotonic_ns() / 1e6} pub {affinity} freq = {freq:.6f} Gb pub / sec")


def sub_array(fd, affinity):
    os.sched_setaffinity(0, affinity)

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind(("127.0.0.1", 5000 + affinity[0]))

    memory = mmap.mmap(fd.get(), frame_size, mmap.MAP_SHARED, mmap.PROT_READ | mmap.PROT_WRITE)
    arr = np.ndarray(shape=SIZE, dtype=np.uint8, buffer=memory)

    data_tensor = np.random.randint(0, 255, size=SIZE, dtype=np.uint8)

    while True:
        data, addr = sock.recvfrom(1024)
        t_start = time.monotonic_ns()
        data_tensor[:] = arr[:]
        t_end = time.monotonic_ns()
        freq = np.prod(data_tensor.shape) * data_tensor.dtype.itemsize / (t_end - t_start)
        # print(data_tensor[:4, :4])
        print(f"{time.monotonic_ns() / 1e6} sub {affinity} freq = {freq:.6f} Gb sub / sec")


if __name__ == '__main__':
    dmaHeap = DmaHeap()
    frame_size = 1
    for r in SIZE:
        frame_size *= r

    start_affinity = 0
    N = 3

    fd = [dmaHeap.alloc(f"test", frame_size) for i in range(N)]
    pub_p = [multiprocessing.Process(target=pub_array, args=(fd[i], [(start_affinity + i * 2) % 32, ])) for i in
             range(N)]
    sub_p = [multiprocessing.Process(target=sub_array, args=(fd[i], [(start_affinity + i * 2 + 1) % 32, ])) for i in
             range(N)]

    print(os.getpid())
    for i in range(N):
        print(fd[i].get())
        pub_p[i].start()
        sub_p[i].start()

    # pub_1_p.join()
    # sub_1_p.join()
    # pub_2_p.join()
    # sub_2_p.join()

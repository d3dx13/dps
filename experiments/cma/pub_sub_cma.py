import multiprocessing
from multiprocessing import shared_memory, sharedctypes
from dma_heap import *
import mmap
import numpy as np
import os
import time
import ctypes


def pub_array(fd, affinity):
    os.sched_setaffinity(0, affinity)

    memory = mmap.mmap(fd.get(), frame_size, mmap.MAP_SHARED, mmap.PROT_READ | mmap.PROT_WRITE)
    arr = np.ndarray(shape=(1024, 1024), dtype=np.uint8, buffer=memory)

    while True:
        data_tensor = np.random.randint(0, 255, size=(1024, 1024), dtype=np.uint8)
        t_start = time.monotonic_ns()
        arr[:] = data_tensor[:]
        t_end = time.monotonic_ns()
        print(
            f"pub {affinity} freq = {np.prod(data_tensor.shape) * data_tensor.dtype.itemsize / (t_end - t_start):.6f} Gb pub / sec")


def sub_array(fd, affinity):
    os.sched_setaffinity(0, affinity)

    memory = mmap.mmap(fd.get(), frame_size, mmap.MAP_SHARED, mmap.PROT_READ | mmap.PROT_WRITE)
    arr = np.ndarray(shape=(1024, 1024), dtype=np.uint8, buffer=memory)

    data_tensor = np.random.randint(0, 255, size=(1024, 1024), dtype=np.uint8)

    while True:
        t_start = time.monotonic_ns()
        data_tensor[:] = arr[:]
        t_end = time.monotonic_ns()
        print(data_tensor[:4, :4])
        print(
            f"sub {affinity} freq = {np.prod(data_tensor.shape) * data_tensor.dtype.itemsize / (t_end - t_start):.6f} Gb pub / sec")


if __name__ == '__main__':
    dmaHeap = DmaHeap()
    frame_size = 1024 * 1024

    start_affinity = 4
    N = 1

    fd = [dmaHeap.alloc(f"test", frame_size) for i in range(N)]
    pub_p = [multiprocessing.Process(target=pub_array, args=(fd[i], [start_affinity + i * 2, ])) for i in range(N)]
    sub_p = [multiprocessing.Process(target=sub_array, args=(fd[i], [start_affinity + i * 2 + 1, ])) for i in range(N)]

    for i in range(N):
        pub_p[i].start()
        sub_p[i].start()

    # pub_1_p.join()
    # sub_1_p.join()
    # pub_2_p.join()
    # sub_2_p.join()

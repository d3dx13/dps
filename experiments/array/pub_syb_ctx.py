import multiprocessing
from multiprocessing import shared_memory, sharedctypes
import numpy as np
import os
import time
import ctypes


def pub_array(data, affinity):
    os.sched_setaffinity(0, affinity)

    data_tensor = np.float32(np.random.random(size=(1024, 128)))

    data_array = np.ndarray(data_tensor.shape, dtype=np.float32, buffer=data.get_obj())

    while True:
        t_start = time.monotonic_ns()
        # with data.get_lock():
        #     data_array[:] = data_tensor[:]
        data_array[:] = data_tensor[:]
        t_end = time.monotonic_ns()
        print(
            f"pub {affinity} freq = {np.prod(tensor.shape) * tensor.dtype.itemsize / (t_end - t_start):.6f} Gb pub / sec")


def sub_array(data, affinity):
    os.sched_setaffinity(0, affinity)

    data_tensor = np.float32(np.random.random(size=(1024, 128)))

    data_array = np.ndarray(data_tensor.shape, dtype=np.float32, buffer=data.get_obj())

    while True:
        t_start = time.monotonic_ns()
        # with data.get_lock():
        #     data_tensor[:] = data_array[:]
        data_tensor[:] = data_array[:]
        t_end = time.monotonic_ns()
        print(
            f"sub {affinity} freq = {np.prod(tensor.shape) * tensor.dtype.itemsize / (t_end - t_start):.6f} Gb sub / sec")


if __name__ == '__main__':
    tensor = np.float32(np.random.random(size=(1024, 128)))

    pub_1_array = sharedctypes.Array(ctypes.c_float, int(np.prod(tensor.shape)))
    pub_1_p = multiprocessing.Process(target=pub_array, args=(pub_1_array, [0, ]))
    pub_1_p.start()
    sub_1_p = multiprocessing.Process(target=sub_array, args=(pub_1_array, [1, ]))
    sub_1_p.start()
    # sub_11_p = multiprocessing.Process(target=sub_array, args=(pub_1_array, [2, ]))
    # sub_11_p.start()

    # pub_2_array = sharedctypes.Array(ctypes.c_float, int(np.prod(tensor.shape)))
    # pub_2_p = multiprocessing.Process(target=pub_array, args=(pub_2_array, [2, ]))
    # pub_2_p.start()
    # sub_2_p = multiprocessing.Process(target=sub_array, args=(pub_2_array, [3, ]))
    # sub_2_p.start()

    # pub_1_p.join()
    # sub_1_p.join()
    # pub_2_p.join()
    # sub_2_p.join()

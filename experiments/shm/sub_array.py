from multiprocessing import shared_memory
from multiprocessing.resource_tracker import unregister
import numpy as np
import os
import time

if __name__ == '__main__':
    os.sched_setaffinity(0, [6, ])

    tensor = np.float32(np.random.random(size=(1024, 1024)))

    shm_data = shared_memory.SharedMemory(name="test", size=np.prod(tensor.shape) * tensor.dtype.itemsize)
    unregister("/test", 'shared_memory')
    pub_array = np.ndarray(shape=tensor.shape, dtype=tensor.dtype, buffer=shm_data.buf)

    while True:
        t_start = time.monotonic_ns()
        tensor[:] = pub_array[:]
        t_end = time.monotonic_ns()
        print(f"sub freq = {np.prod(tensor.shape) * tensor.dtype.itemsize / (t_end - t_start):.6f} Gb pub / sec")

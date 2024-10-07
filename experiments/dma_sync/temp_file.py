import ctypes

import sys

sys.path.append("/home/d3dx13/workspace/ips/experiments/dma_proxy/build/lib.linux-x86_64-3.10")
sys.path.append("/home/d3dx13/workspace/ips/experiments/dma_proxy/build/lib.linux-aarch64-cpython-311")
sys.path.append("/home/d3dx13/workspace/ips/experiments/dma_proxy/build/lib.linux-x86_64-cpython-310")

from time import sleep, monotonic
import dma_proxy
import os
import tempfile

print("PID: ", os.getpid())

BASE = "/dev/shm"
PATH = f"{BASE}/ips/node"
NAME = str(os.getpid())

os.system(f"rm -rf {PATH}/*")
os.makedirs(f"{BASE}/ips/node", exist_ok=True)

print("Start")
os.system(f"tree -a {BASE}")

print(f"New file {PATH}/{NAME}")
f = open(f"{PATH}/{NAME}", mode="w")
os.system(f"tree -a {BASE}")

ctypes.string_at(0)

import ctypes

import sys

sys.path.append("/home/d3dx13/workspace/ips/experiments/dma_proxy/build/lib.linux-x86_64-3.10")
sys.path.append("/home/d3dx13/workspace/ips/experiments/dma_proxy/build/lib.linux-aarch64-cpython-311")
sys.path.append("/home/d3dx13/workspace/ips/experiments/dma_proxy/build/lib.linux-x86_64-cpython-310")

from time import sleep, monotonic
import dma_proxy
import os

BASE = "/dev/shm"
PATH = f"{BASE}/ips/node/{os.getpid()}"
NAME = 1231212

os.system(f"rm -rf {BASE}/*")
os.makedirs(f"{BASE}/ips/node", exist_ok=True)

print("Start")
os.makedirs(PATH, exist_ok=True)
os.system(f"tree -a {BASE}")

print(f"New file {PATH}/{NAME}")
os.system(f"mktemp {PATH}/{NAME}.XXXXXX")
# os.unlink
# f = os.open(f"{PATH}/{NAME}", os.O_RDWR | os.O_CREAT)
os.system(f"tree -a {BASE}")

# ctypes.string_at(0)
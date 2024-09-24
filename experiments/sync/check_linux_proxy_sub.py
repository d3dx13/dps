import sys

sys.path.append("/home/d3dx13/workspace/ips/experiments/linux_proxy/build/lib.linux-aarch64-cpython-311")

import linux_proxy
import os
from time import sleep
import mmap

print("PID:", os.getpid())

pid_input = int(input("Enter PID: "))
fd_input = int(input("Enter fd: "))

fd = linux_proxy.borrow_fd_from_pid(pid_input, fd_input)
memory = mmap.mmap(fd, 1024 * 1024 * 128, mmap.MAP_SHARED, mmap.PROT_READ | mmap.PROT_WRITE)

while True:
    print(fd, memory[:100])
    sleep(1.0)
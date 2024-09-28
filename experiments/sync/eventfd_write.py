import sys

sys.path.append("/home/d3dx13/workspace/ips/experiments/linux_proxy/build/lib.linux-x86_64-cpython-310")

"""
pos:  0
flags:  02004002
mnt_id:  15
ino:  59
eventfd-count:                0
eventfd-id: 32
eventfd-semaphore: 0
"""

EFD_SEMAPHORE = 0x00000001
EFD_CLOEXEC = 0x02000000
EFD_NONBLOCK = 0x00004000

MFD_CLOEXEC = 1
MFD_ALLOW_SEALING = 2
MFD_HUGETLB = 4

import linux_proxy
import os
from time import sleep
import select

print("PID:", os.getpid())

fd = linux_proxy.eventfd(0, 0) # EFD_SEMAPHORE | EFD_CLOEXEC | EFD_NONBLOCK

while True:
    value = int(input("> "))
    if value >= 0:
        linux_proxy.write(fd, value)
    else:
        linux_proxy.read(fd, 1)
    os.system(f"cat /proc/{os.getpid()}/fdinfo/{fd}")
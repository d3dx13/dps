import sys

sys.path.append("/home/d3dx13/workspace/ips/experiments/linux_proxy/build/lib.linux-x86_64-cpython-310")

import linux_proxy
import os
from time import sleep
import select

print("PID:", os.getpid())

pid_input = int(input("Enter PID: "))
fd_input = int(input("Enter fd: "))

fd = linux_proxy.borrow_fd_from_pid(pid_input, fd_input)
print("borrow_fd_from_pid:", fd)

event = select.epoll(sizehint=-1, flags=0)
event.register(fd=fd, eventmask=select.EPOLLIN | select.EPOLLET)

while True:
    events = event.poll()
    print(events)
    # sleep(1.0)
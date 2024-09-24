import sys

sys.path.append("/home/d3dx13/workspace/ips/experiments/linux_proxy/build/lib.linux-aarch64-cpython-311")

import linux_proxy
import os
from time import sleep, monotonic
import select

print("PID:", os.getpid())

pid_input = int(input("Enter PID: "))
fd_input = 7

fd = linux_proxy.borrow_fd_from_pid(pid_input, fd_input)

event = select.epoll(sizehint=-1, flags=0)
event.register(fd=fd, eventmask=select.EPOLLIN | select.EPOLLET)

results = set()

while True:
    events = event.poll()
    results.add(tuple(events))
    print(monotonic(), events, results)
    # sleep(1.0)
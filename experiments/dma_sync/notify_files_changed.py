import sys

sys.path.append("/home/d3dx13/workspace/ips/experiments/dma_proxy/build/lib.linux-x86_64-3.10")
sys.path.append("/home/d3dx13/workspace/ips/experiments/dma_proxy/build/lib.linux-aarch64-cpython-311")
sys.path.append("/home/d3dx13/workspace/ips/experiments/dma_proxy/build/lib.linux-x86_64-cpython-310")
sys.path.append("/home/d3dx13/workspace/ips/experiments/linux_proxy/build/lib.linux-x86_64-3.10")
sys.path.append("/home/d3dx13/workspace/ips/experiments/linux_proxy/build/lib.linux-aarch64-cpython-311")
sys.path.append("/home/d3dx13/workspace/ips/experiments/linux_proxy/build/lib.linux-x86_64-cpython-310")

import os
from time import sleep, monotonic
import dma_proxy
import linux_proxy
import mmap
import datetime
import select
import numpy as np

# Supported events suitable for MASK parameter of INOTIFY_ADD_WATCH.  */
IN_ACCESS=0x00000001	# File was accessed.  */
IN_MODIFY=0x00000002	# File was modified.  */
IN_ATTRIB=0x00000004	# Metadata changed.  */
IN_CLOSE_WRITE=0x00000008	# Writtable file was closed.  */
IN_CLOSE_NOWRITE=0x00000010	# Unwrittable file closed.  */
IN_CLOSE=(IN_CLOSE_WRITE | IN_CLOSE_NOWRITE) # Close.  */
IN_OPEN	=0x00000020	# File was opened.  */
IN_MOVED_FROM=0x00000040	# File was moved from X.  */
IN_MOVED_TO=0x00000080	# File was moved to Y.  */
IN_MOVE	=(IN_MOVED_FROM | IN_MOVED_TO) # Moves.  */
IN_CREATE=0x00000100	# Subfile was created.  */
IN_DELETE=0x00000200	# Subfile was deleted.  */
IN_DELETE_SELF=0x00000400	# Self was deleted.  */
IN_MOVE_SELF=0x00000800	# Self was moved.  */

# Events sent by the kernel.  */
IN_UNMOUNT=0x00002000	# Backing fs was unmounted.  */
IN_Q_OVERFLOW=0x00004000	# Event queued overflowed.  */
IN_IGNORED=0x00008000	# File was ignored.  */

# Helper events.  */
IN_CLOSE=(IN_CLOSE_WRITE | IN_CLOSE_NOWRITE)	# Close.  */
IN_MOVE	=(IN_MOVED_FROM | IN_MOVED_TO)		# Moves.  */

# Special flags.  */
IN_ONLYDIR=0x01000000	# Only watch the path if it is a directory.  */
IN_DONT_FOLLOW=0x02000000	# Do not follow a sym link.  */
IN_EXCL_UNLINK=0x04000000	# Exclude events on unlinked objects.  */
IN_MASK_CREATE=0x10000000	# Only create watches.  */
IN_MASK_ADD=0x20000000	# Add to the mask of an already existing watch.  */
IN_ISDIR=0x40000000	# Event occurred against dir.  */
IN_ONESHOT=0x80000000	# Only send event once.  */

#define IN_CLOEXEC IN_CLOEXEC
IN_CLOEXEC = 0o02000000
IN_NONBLOCK = 0o00004000

import resource
print(resource.getrlimit(resource.RLIMIT_NOFILE))
print(resource.setrlimit(resource.RLIMIT_NOFILE, (1048576, 1048576)))

event = select.epoll(sizehint=-1, flags=0)

# mask = IN_DELETE | IN_CLOSE | IN_MODIFY
mask = IN_CREATE | IN_DELETE | IN_OPEN | IN_CLOSE | IN_MODIFY

fds = []
paths = [
    "/dev/shm/.dps/node",
    "/dev/shm/.dps/topic",
]

inotify_fd = linux_proxy.inotify_init1(0)
print("inotify_fd", inotify_fd)
for path_str in paths:
    print(path_str)
    fds.append(linux_proxy.inotify_add_watch(inotify_fd, path_str, mask))

for fd in fds:
    print(fd)
    event.register(fd=fd)

while True:
    events = event.poll()
    print()
    print("Event", events)
    for event_pair in events:
        for fd in fds:
            if fd == event_pair[0]:
                temp = linux_proxy.inotify_event_read(inotify_fd)
                print("HALP", event_pair, fd, temp)
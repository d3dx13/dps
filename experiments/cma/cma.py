import ctypes
import fcntl
import logging
import os


# Libcamera C++ classes
class UniqueFD:
    """Libcamera UniqueFD Class"""

    def __init__(self, fd=-1):
        if isinstance(fd, UniqueFD):
            self.__fd = fd.release()
        else:
            self.__fd = fd

    def release(self):
        fd = self.__fd
        self.__fd = -1
        return fd

    def get(self):
        return self.__fd

    def isValid(self):
        return self.__fd >= 0


class DmaHeap:
    """DmaHeap"""

    def __init__(self):
        self.__dmaHeapHandle = UniqueFD()
        for name in heapNames:
            try:
                print(f"Opening {name} | {os.O_CLOEXEC | os.O_RDWR}")
                ret = os.open(name, os.O_CLOEXEC | os.O_RDWR | os.O_NONBLOCK)
                print(f"ret {ret}")
                print(f"type(ret) {type(ret)}")
            except FileNotFoundError:
                _log.info(f"Failed to open {name}")
                continue

            self.__dmaHeapHandle = UniqueFD(ret)
            break

        if not self.__dmaHeapHandle.isValid():
            raise RuntimeError("Could not open any dmaHeap device")

    @property
    def isValid(self):
        return self.__dmaHeapHandle.isValid()

    def alloc(self, name, size) -> UniqueFD:
        alloc = dma_heap_allocation_data()
        alloc.len = size
        alloc.fd_flags = os.O_CLOEXEC | os.O_RDWR
        ret = fcntl.ioctl(self.__dmaHeapHandle.get(), DMA_HEAP_IOCTL_ALLOC, alloc)
        if ret < 0:
            _log.error(f"dmaHeap allocation failure for {name}")
            return UniqueFD()

        allocFd = UniqueFD(alloc.fd)
        print(f"alloc.fd {alloc.fd}")
        print(f"allocFd.get() {allocFd.get()}")

        ret = fcntl.ioctl(allocFd.get(), DMA_BUF_SET_NAME, name)
        if not isinstance(ret, bytes) and ret < 0:
            _log.error(f"dmaHeap naming failure for {name}")
            return UniqueFD()

        return allocFd

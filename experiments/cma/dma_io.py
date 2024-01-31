import ctypes

from v4l2_io import _IOW, _IOWR

heapNames = [
    "/dev/dma_heap/reserved"
]


# Kernel stuff from linux/dma-buf.h
class dma_buf_sync(ctypes.Structure):
    _fields_ = [
        ('flags', ctypes.c_uint64),
    ]


DMA_BUF_SYNC_READ = (1 << 0)
DMA_BUF_SYNC_WRITE = (2 << 0)
DMA_BUF_SYNC_RW = (DMA_BUF_SYNC_READ | DMA_BUF_SYNC_WRITE)
DMA_BUF_SYNC_START = (0 << 2)
DMA_BUF_SYNC_END = (1 << 2)

DMA_BUF_BASE = 'b'
DMA_BUF_IOCTL_SYNC = _IOW(DMA_BUF_BASE, 0, dma_buf_sync)

DMA_BUF_SET_NAME = _IOW(DMA_BUF_BASE, 1, ctypes.c_char_p)


# Kernel stuff from linux/dma-heap.h
class dma_heap_allocation_data(ctypes.Structure):
    _fields_ = [
        ('len', ctypes.c_uint64),
        ('fd', ctypes.c_uint32),
        ('fd_flags', ctypes.c_uint32),
        ('heap_flags', ctypes.c_uint64),
    ]


DMA_HEAP_IOC_MAGIC = 'H'

DMA_HEAP_IOCTL_ALLOC = _IOWR(DMA_HEAP_IOC_MAGIC, 0, dma_heap_allocation_data)

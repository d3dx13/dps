#pragma once

#include <map>
#include <string>
#include <sys/syscall.h>
#include <unistd.h>
#include <filesystem>
#include <fcntl.h>

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>

#include <linux/dma-buf.h>
#include <linux/dma-heap.h>
#include <drm/drm.h>

class Allocator {
public:
    enum memory {
        RESERVED,
        LINUX_CMA,
        SYSTEM,
    };

    Allocator();

    Allocator(memory);

    ~Allocator();

protected:
    std::map<memory, std::string> memory_path = {
            {memory::RESERVED, "/dev/dma_heap/reserved"},
            {memory::LINUX_CMA, "/dev/dma_heap/linux,cma"},
            {memory::SYSTEM, "/dev/dma_heap/system"}
    };

    bool is_dma_heap_valid(std::string);
    int dmabuf_heap_open(std::string);
    int borrow_fd_from_pid(pid_t, int);

    int shm_open();

    int dma_heap_fd;
    memory memory_space;
};


#include "allocator.h"
#include "exceptions.h"

#include <iostream>
#include <vector>

Allocator::Allocator(Allocator::memory memory_space) {
    // Trying to find dma_heap
    if (!std::filesystem::exists(this->memory_path[memory_space])) {
        throw exceptions::DmaBufferNotFound(this->memory_path[memory_space]);
    }

    // Trying to open dma_heap
    this->memory_space = memory_space;
    this->dma_heap_fd = dmabuf_heap_open(this->memory_path[this->memory_space]);
    if (this->dma_heap_fd < 0) {
        throw exceptions::DmaBufferNoUserRights(this->memory_path[memory_space]);
    }
}

Allocator::Allocator() {
    bool valid_memory_path_found = false; // Flag that a file descriptor was successfully created on dma_heap
    std::vector<std::string> memory_path_exists; // List of dma_heap that exist, but to which there are no access rights

    // Trying to open first accessible dma_heap
    for (std::pair<memory, std::string> mem: this->memory_path) {
        if (!std::filesystem::exists(mem.second)) {
            continue;
        }
        memory_path_exists.push_back(mem.second);
        this->memory_space = mem.first;
        this->dma_heap_fd = dmabuf_heap_open(this->memory_path[this->memory_space]);
        if (this->dma_heap_fd < 0) {
            continue;
        }
        valid_memory_path_found = true;
        break;
    }

    if (!valid_memory_path_found) { // Handle scenarios when no dma_heap could be opened
        if (memory_path_exists.empty()) {
            throw exceptions::DmaBufferNotFound("No One");
        } else {
            std::cout << "DMA Buffer Found Without Rights :\n";
            for (const std::string &path: memory_path_exists) {
                std::cout << path << "\n";
            }
            throw exceptions::DmaBufferNoUserRights(memory_path_exists[0]);
        }
    } else { // Successful opening of the first dma_heap
        std::cout << "DMA Buffer Auto Found : " << this->memory_path[this->memory_space] << "\n";;
    }
}

Allocator::~Allocator() {
    // Closing dma_heap
    if (this->dma_heap_fd >= 0) {
        close(this->dma_heap_fd);
    }
}

int Allocator::dmabuf_heap_open(const std::string &path) {
    return (int) open(path.c_str(), O_RDWR);
}

int Allocator::borrow_fd_from_pid(pid_t pid, int fd) {
    int pid_fd = (int) syscall(SYS_pidfd_open, pid, 0); // PIDFD_NONBLOCK
    return (int) syscall(SYS_pidfd_getfd, pid_fd, fd, 0); // PTRACE_MODE_ATTACH_REALCREDS
}

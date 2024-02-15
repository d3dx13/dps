#include "allocator.h"
#include "exceptions.h"

#include <iostream>
#include <vector>

Allocator::Allocator(Allocator::memory memory_space) {
    if (!std::filesystem::exists(this->memory_path[memory_space])) {
        throw exceptions::DmaBufferNotFound(this->memory_path[memory_space]);
    }
    this->memory_space = memory_space;
    this->dma_heap_fd = dmabuf_heap_open(this->memory_path[this->memory_space]);
    if (this->dma_heap_fd < 0) {
        throw exceptions::DmaBufferNoUserRights(this->memory_path[memory_space]);
    }
}

Allocator::Allocator() {
    bool valid_memory_path_found = false;
    std::vector<std::string> memory_path_exists;
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
    if (!valid_memory_path_found) {
        if (memory_path_exists.empty()) {
            throw exceptions::DmaBufferNotFound("No One");
        } else {
            std::cout << "DMA Buffer Found Without Rights :\n";
            for (const std::string &path: memory_path_exists) {
                std::cout << path << "\n";
            }
            throw exceptions::DmaBufferNoUserRights(memory_path_exists[0]);
        }
    } else {
        std::cout << "DMA Buffer Auto Found : " << this->memory_path[this->memory_space] << "\n";;
    }
}

Allocator::~Allocator() {

}

int Allocator::dmabuf_heap_open(const std::string &path) {
    return (int) open(path.c_str(), O_RDWR);
}

int Allocator::borrow_fd_from_pid(pid_t pid, int fd) {
    int pid_fd = (int) syscall(SYS_pidfd_open, pid, 0); // PIDFD_NONBLOCK
    return (int) syscall(SYS_pidfd_getfd, pid_fd, fd, 0); // PTRACE_MODE_ATTACH_REALCREDS
}

#include "allocator.h"
#include "exceptions.h"

#include <iostream>

Allocator::Allocator(Allocator::memory memory_space) {
    if (!Allocator::is_dma_heap_valid(this->memory_path[memory_space])) {
        throw exceptions::DmaBufferNotFound(this->memory_path[memory_space]);
    }
    this->memory_space = memory_space;
}

Allocator::Allocator() {
    bool memory_path_found = false;
    for (std::pair<memory, std::string> mem: this->memory_path) {
        if (Allocator::is_dma_heap_valid(mem.second)) {
            this->memory_space = mem.first;
            memory_path_found = true;
            break;
        }
    }
    if (!memory_path_found) {
        throw exceptions::DmaBufferNotFound("No One");
    } else {
        std::cout << "DMA Buffer Auto Found : " << this->memory_path[this->memory_space] << "\n";;
    }

    this->dma_heap_fd = dmabuf_heap_open(this->memory_path[this->memory_space]);
    if (this->dma_heap_fd < 0) {

    }
    std::cout << this->dma_heap_fd << "\n";;

    // this->memory_space = memory_space;
}

Allocator::~Allocator() {

}

bool Allocator::is_dma_heap_valid(std::string path) {
    std::filesystem::perms perms = std::filesystem::status(path).permissions();
    bool can_read = (
            (perms & std::filesystem::perms::owner_read) != std::filesystem::perms::none ||
            (perms & std::filesystem::perms::group_read) != std::filesystem::perms::none ||
            (perms & std::filesystem::perms::others_read) != std::filesystem::perms::none
    );
    std::cout << std::filesystem::exists(path) << " - " << (int) perms << ": " << can_read << "\n";
    return std::filesystem::exists(path) > 0;
}

int Allocator::dmabuf_heap_open(std::string path) {
    int fd = open(path.c_str(), O_RDWR);
}

int Allocator::borrow_fd_from_pid(pid_t pid, int fd) {
    int pid_fd = (int) syscall(SYS_pidfd_open, pid, 0); // PIDFD_NONBLOCK
    return (int) syscall(SYS_pidfd_getfd, pid_fd, fd, 0); // PTRACE_MODE_ATTACH_REALCREDS
}

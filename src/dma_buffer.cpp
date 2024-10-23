#include "dps/core/common.h"
#include "dps/core/dma_buffer.h"

namespace dps {
    DMABuffer::DMABuffer(size_t size, std::string heap_name, std::string buffer_name) {
        this->allocate(size, heap_name, buffer_name);
        this->map(false);
    }

    DMABuffer::DMABuffer(int pid, int fd) : DMABuffer(pid, fd, true) {}
    DMABuffer::DMABuffer(int pid, int fd, bool readonly) {
        this->connect(pid, fd);
        this->map(readonly);
    }

    DMABuffer::~DMABuffer() {
        /*
        Close dma_buff
        */
        close(this->dma_buf_fd);
        this->dma_buf_fd = -1;

        /*
        unmap memory
        */
        this->unmap();
        
        /*
        Reset local variables to default
        */
        this->_size = 0;
        this->heap_name = "";
        this->buffer_name = "";
    }

    void DMABuffer::allocate(size_t size, std::string heap_name, std::string buffer_name){
        /*
        Open DMA Heap to allocate through
        */
        int heap_fd = open((DMA_HEAP_PATH + heap_name).c_str(), O_RDONLY | O_CLOEXEC);
        if (heap_fd < 0) {
            std::cerr << "open dma heap failed, heap: " << heap_name.c_str() << "\n";
            raise(SIGSEGV);
        }

        /*
        Allocate DMA Buffer with size != 0
        */
        struct dma_heap_allocation_data heap_data = {};
        heap_data.len = size;
        heap_data.fd_flags = O_RDWR | O_CLOEXEC;
        if (ioctl(heap_fd, DMA_HEAP_IOCTL_ALLOC, &heap_data) != 0) {
            std::cerr << "dma heap alloc failed, len: " << heap_data.len << "\n";
            close(heap_fd);
            raise(SIGSEGV);
        }
        close(heap_fd); // close DMA Heap

        /*
        Rename DMA Buffer
        */
        char dma_buf_name[DMA_BUF_NAME_LEN - 1] = {};
        for (int i = 0; i < std::min(sizeof(dma_buf_name), buffer_name.length()); i++){
            dma_buf_name[i] = buffer_name[i];
        }
        if (ioctl(heap_data.fd, DMA_BUF_SET_NAME, dma_buf_name) != 0) {
            std::cerr << "dma buf rename failed, name: " << dma_buf_name << "\n";
        }

        this->dma_buf_fd = heap_data.fd;
        this->update_buffer_info();
    }

    void DMABuffer::connect(int pid, int fd) {
        /*
        Open PID fd
        */
        int pid_fd = syscall(SYS_pidfd_open, pid, 0); // PIDFD_NONBLOCK
        if (pid_fd < 0) {
            std::cerr << "SYS_pidfd_open " << pid << " failed\n";
            raise(SIGSEGV);
        }

        /*
        Get DMA Buffer from PID
        */
        int dma_buf_fd = syscall(SYS_pidfd_getfd, pid_fd, fd, 0); // PTRACE_MODE_ATTACH_REALCREDS
        if (dma_buf_fd < 0) {
            std::cerr << "SYS_pidfd_getfd fd " << fd << " from pid " << pid << " failed\n";
            raise(SIGSEGV);
        }
        close(pid_fd);

        this->dma_buf_fd = dma_buf_fd;
        this->update_buffer_info();
    }

    void DMABuffer::update_buffer_info() {
        /*
        Read DMA Buffer public information
        */
        std::string heap_name = "";
        std::string buffer_name = "";
        size_t size = 0;
        std::ifstream fd_info("/proc/self/fdinfo/" + std::to_string(this->dma_buf_fd));
        if(!fd_info.good()){
            std::cerr << "read fdinfo failed: " << this->dma_buf_fd << "\n";
            raise(SIGSEGV);
        }
        std::string line;
        while (std::getline(fd_info, line)) {
            std::string field_name = line.substr(0, line.find(':'));
            std::string field_value = line.substr(line.find(':') + 1);
            dps_common::trim(field_name);
            dps_common::trim(field_value);

            if (field_name.find("size") == 0) {
                size = std::stoul(field_value);
            } else if (field_name.find("exp_name") == 0) {
                heap_name = field_value;
            } else if (field_name.find("name") == 0) {
                buffer_name = field_value;
            }
        }
        fd_info.close(); // close fdinfo file

        /*
        We must have DMA Buffer info and DMA Buffer can not have 0 length
        */
        if (!size) {
            std::cerr << "read fdinfo size failed " << this->dma_buf_fd << "\n";
            raise(SIGSEGV);
        }

        /*
        Save class variables
        */
        this->heap_name = heap_name;
        this->buffer_name = buffer_name;
        this->_size = size;
    }

    void DMABuffer::map(bool readonly) {
        /*
        Map Buffer
        
        nmap is split into 2 separate calls so that it is not possible to enter the Header address space from the buffer without Seg Falt
        */
        this->_buffer = (uint8_t *) mmap(NULL, this->_size, 
            readonly ? PROT_READ : PROT_READ | PROT_WRITE, 
            MAP_SHARED, this->dma_buf_fd, 0
        );
        if (this->_buffer == MAP_FAILED) {
            std::cerr << "dma_buff mmap body_mem failed, fd: " << this->dma_buf_fd << "\n";
            raise(SIGSEGV);
        }
    }
    
    void DMABuffer::unmap() {
        if (this->_buffer != nullptr) {
            munmap(this->_buffer, this->_size);
        }
        this->_buffer = nullptr;
    }

    int DMABuffer::fd() {
        return this->dma_buf_fd;
    }

    size_t DMABuffer::size() {
        return this->_size;
    }

    uint8_t * DMABuffer::buffer() {
        return this->_buffer;
    }

    std::string DMABuffer::get_heap_name() {
        return this->heap_name;
    }

    std::string DMABuffer::get_buffer_name() {
        return this->buffer_name;
    }
}
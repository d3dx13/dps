#include "dps/core/common.h"
#include "dps/core/dma_buffer.h"

namespace dps {
    DMABuffer::DMABuffer(size_t size, std::string heap_name, std::string buffer_name, std::string file_association_name) : DMABuffer(size, heap_name, buffer_name) {
        this->file_association_name = file_association_name;
        this->create_file();
    }
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
        Remove file association
        */
        this->delete_file();
        
        /*
        Reset local variables to default
        */
        this->message_size = 0;
        this->full_size = 0;
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
        heap_data.len = PAGE_SIZE + size;
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

        this->message_size = size;
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
        this->full_size = size;
    }

    void DMABuffer::map(bool readonly) {
        /*
        Map Header

        Since we can't always control the offset in memory when we pass dma_buff to the driver, 
        the header is placed as an additional PAGE of 4096 bytes at the end of the buffer. 
        */
        this->_header = (dma_buffer_header *) mmap(NULL, PAGE_SIZE, 
            readonly ? PROT_READ : PROT_READ | PROT_WRITE, 
            MAP_SHARED, this->dma_buf_fd, this->full_size - PAGE_SIZE
        );
        if (this->_header == MAP_FAILED) {
            std::cerr << "dma_buff mmap header_mem failed, fd: " << this->dma_buf_fd << "\n";
            raise(SIGSEGV);
        }
        /*
        Get and Provide information from the header that was not available before Header mmap()
        */
        if (!readonly) {
            this->_header->size = this->message_size;
        } else {
            this->message_size = this->_header->size;
        }
        
        /*
        Map Buffer
        
        nmap is split into 2 separate calls so that it is not possible to enter the Header address space from the buffer without Seg Falt
        */
        if (this->full_size - PAGE_SIZE != 0) {
            this->_buffer = (uint8_t *) mmap(NULL, this->full_size - PAGE_SIZE, 
                readonly ? PROT_READ : PROT_READ | PROT_WRITE, 
                MAP_SHARED, this->dma_buf_fd, 0
            );
            if (this->_buffer == MAP_FAILED) {
                std::cerr << "dma_buff mmap body_mem failed, fd: " << this->dma_buf_fd << "\n";
                raise(SIGSEGV);
            }
        }
    }
    
    void DMABuffer::unmap() {
        if (this->_header != nullptr) {
            munmap(this->_header, PAGE_SIZE);
        }
        this->_header = nullptr;
        if (this->_buffer != nullptr) {
            munmap(this->_buffer, this->full_size - PAGE_SIZE);
        }
        this->_buffer = nullptr;
    }

    void DMABuffer::create_file() {
        this->file_association_path = std::filesystem::path(this->file_association_name);

        if (!std::filesystem::exists(this->file_association_path.parent_path()) or !std::filesystem::is_directory(this->file_association_path.parent_path())) {
            std::filesystem::create_directories(this->file_association_path.parent_path());
        }

        std::ofstream {this->file_association_path};
    }

    void DMABuffer::delete_file() {
        if (this->file_association_name.length() > 0) {
            std::filesystem::remove(this->file_association_path);
            this->file_association_name = "";
        }
    }

    int DMABuffer::fd() {
        return this->dma_buf_fd;
    }

    size_t DMABuffer::size() {
        return this->_header->size;
    }

    uint8_t * DMABuffer::buffer() {
        return this->_buffer;
    }

    dma_buffer_header * DMABuffer::header() {
        return this->_header;
    }

    std::string DMABuffer::get_heap_name() {
        return this->heap_name;
    }

    std::string DMABuffer::get_buffer_name() {
        return this->buffer_name;
    }
}
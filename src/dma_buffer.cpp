#include "dps/core/config.h"
#include "dps/core/dma_buffer.h"

namespace dps {
    DMABuffer::DMABuffer() {

    }

    DMABuffer::~DMABuffer() {
        this->release();
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

        /*
        Save class variables
        */
        this->fd = heap_data.fd;
        this->heap_name = heap_name;
        this->buffer_name = std::string(dma_buf_name);
        this->size = size;
    }

    // trim from start (in place)
    inline void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
    }

    // trim from end (in place)
    inline void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    // trim from both ends (in place)
    inline void trim(std::string &s) {
        rtrim(s);
        ltrim(s);
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

        /*
        Read DMA Buffer public information
        */
        std::string heap_name = "";
        std::string buffer_name = "";
        size_t size = 0;
        std::ifstream fd_info("/proc/self/fdinfo/" + std::to_string(dma_buf_fd));
        if(!fd_info.good()){
            std::cerr << "read fdinfo failed: " << fd << "\n";
            raise(SIGSEGV);
        }
        std::string line;
        while (std::getline(fd_info, line)) {
            std::string field_name = line.substr(0, line.find(':'));
            std::string field_value = line.substr(line.find(':') + 1);
            trim(field_name);
            trim(field_value);

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
            std::cerr << "read fdinfo size failed " << fd << "\n";
            raise(SIGSEGV);
        }

        /*
        Save class variables
        */
        this->fd = dma_buf_fd;
        this->heap_name = heap_name;
        this->buffer_name = buffer_name;
        this->size = size;
    }

    void DMABuffer::release() {
        if (this->fd > 0) {
            close(this->fd);

            this->fd = -1;
            this->heap_name = "";
            this->buffer_name = "";
            this->size = 0;

            // TODO add unmap to this->buffer
        }
    }

    int DMABuffer::get_fd(){
        return this->fd;
    }
}
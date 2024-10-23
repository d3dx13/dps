#ifndef DMA_BUFFER_H
#define DMA_BUFFER_H

#include "dps/core/common.h"

namespace dps {
    class DMABuffer {
        public:
            DMABuffer(size_t size, std::string heap_name, std::string buffer_name);
            DMABuffer(int pid, int fd, bool readonly);
            DMABuffer(int pid, int fd);
            ~DMABuffer();
            
            int fd();
            size_t size();
            uint8_t * buffer();

            std::string get_heap_name();
            std::string get_buffer_name();
        
        protected:
            void allocate(size_t size, std::string heap_name, std::string buffer_name);
            void connect(int pid, int fd);

            void map(bool readonly);
            void unmap();

            size_t _size;

            std::string heap_name;
            std::string buffer_name;

            int dma_buf_fd = -1;
            uint8_t * _buffer;

            void update_buffer_info();

            const int pid = getpid();
    };
}

#endif
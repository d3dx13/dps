#include "dps/core/config.h"

namespace dps {
    struct dma_buffer_header {
        size_t size;
        unsigned long msg_id;
        // TODO add supported sync flags
    };

    class DMABuffer {
        public:
            DMABuffer(size_t size, std::string heap_name, std::string buffer_name);
            DMABuffer(int pid, int fd);
            ~DMABuffer();
            
            int fd();
            size_t size();
            uint8_t * buffer();
            dma_buffer_header * header();

            std::string get_heap_name();
            std::string get_buffer_name();
        
        protected:
            void allocate(size_t size, std::string heap_name, std::string buffer_name);
            void connect(int pid, int fd);
            void release();

            void map(bool readonly);
            void unmap();

            size_t full_size;
            size_t message_size;

            std::string heap_name;
            std::string buffer_name;

            int dma_buf_fd = -1;
            uint8_t * _buffer;
            dma_buffer_header * _header;

            void update_buffer_info();

            const int pid = getpid();
    };
}
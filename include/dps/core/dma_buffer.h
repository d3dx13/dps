#include "dps/core/config.h"

namespace dps {
    struct dma_buffer_header {
        size_t size; // TODO support
        unsigned long msg_id;
        // TODO add supported sync flags
    };

    class DMABuffer {
        public:
            DMABuffer();
            ~DMABuffer();

            void allocate(size_t size, std::string heap_name, std::string buffer_name);
            void connect(int pid, int fd);
            void release();

            void map();
            void unmap();
            
            int get_fd();
            std::unique_ptr<uint8_t> get_buffer();
            size_t get_size();
            std::string get_heap_name();
            std::string get_buffer_name();

            uint8_t * buffer;
            dma_buffer_header * header;
        
        protected:
            size_t size;
            std::string heap_name;
            std::string buffer_name;

            int fd = -1;

            void update_buffer_info();

            const int pid = getpid();
    };
}
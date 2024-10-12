#include "dps/core/config.h"

namespace dps {
    class DMABuffer {
        public:
            DMABuffer();
            ~DMABuffer();

            void allocate(size_t size, std::string heap_name, std::string buffer_name);
            void connect(int pid, int fd);
            void release();

            void map();
            void unmap();
            
            std::unique_ptr<uint8_t> get_buffer();
            int get_fd();
            size_t get_size();
            std::string get_heap_name();
            std::string get_buffer_name();
        
        protected:
            size_t size;
            std::string heap_name;
            std::string buffer_name;

            std::unique_ptr<uint8_t> buffer;
            int fd = -1;

            const int pid = getpid();
    };
}
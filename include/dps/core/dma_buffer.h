#include "dps/core/config.h"

namespace dps {
    struct dma_buffer_header {
        size_t size;
        unsigned long msg_id;
        // TODO add supported sync flags
    };

    class DMABuffer {
        public:
            DMABuffer(size_t size, std::string heap_name, std::string buffer_name); // , std::string file_name -> @{pid}:fd:index:max_index
            DMABuffer(int pid, int fd, bool readonly);
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
            void create_file(); // TODO

            void map(bool readonly);
            void unmap();
            void delete_file(); // TODO

            size_t full_size;
            size_t message_size;

            std::string heap_name;
            std::string buffer_name;
            std::string file_name;
            std::filesystem::path file_path;

            int dma_buf_fd = -1;
            uint8_t * _buffer;
            dma_buffer_header * _header;

            void update_buffer_info();

            const int pid = getpid();
    };
}
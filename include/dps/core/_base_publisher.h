#include "dps/core/common.h"

namespace dps
{
    class BasePublisher
    {
    public:
        BasePublisher(std::string topic_name, size_t message_size, int queue_size, std::string heap_name);

        ~BasePublisher();

        void publish();

        std::unique_ptr<uint8_t> buffer();
        std::unique_ptr<int> fd();

        const size_t message_size;
        const int queue_size;
        const std::string heap_name;
        const std::string topic_name;

    protected:
        const int pid = getpid();
        std::filesystem::path topic_path;

        size_t index;
        std::vector<std::unique_ptr<uint8_t>> mapped_buffers;
        std::vector<int> allocated_dma_bufs_fd;

    private:
        std::filesystem::path init_filesystem();
        void cleanup_filesystem();

        std::vector<int> allocate_dma_bufs();
        int allocate(size_t buffer_size);

        std::vector<std::unique_ptr<uint8_t>> map_buffers();
    };
}
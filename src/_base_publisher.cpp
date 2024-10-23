#include "dps/core/common.h"
#include "dps/core/base_publisher.h"

namespace dps
{
    BasePublisher::BasePublisher(std::string topic_name, size_t message_size, int queue_size, std::string heap_name)
        : message_size(message_size), queue_size(queue_size), heap_name(heap_name), topic_name(dps_common::simplify_topic_name(topic_name))
    {
        std::cout << "New Pub" << topic_name << " : " << message_size << " : " << queue_size << " : " << heap_name << "\n";

        this->topic_path = this->init_filesystem();
        this->allocated_dma_bufs_fd = this->allocate_dma_bufs();
        this->mapped_buffers = this->map_buffers();

        std::cout << "s1: " << this->allocated_dma_bufs_fd.size() << "\n";
        std::cout << "s2: " << this->mapped_buffers.size() << "\n";
        for (auto fd : this->allocated_dma_bufs_fd)
        {
            std::cout << fd << "\n";
        }
    }

    BasePublisher::~BasePublisher()
    {
        this->cleanup_filesystem();
    }

    void BasePublisher::publish()
    {
    }

    std::filesystem::path BasePublisher::init_filesystem()
    {
        std::filesystem::path base_path = std::filesystem::path(DPS_BASE_PATH) / DPS_TOPIC_PATH;

        if (!std::filesystem::exists(base_path) or !std::filesystem::is_directory(base_path))
        {
            std::cout << "Recreating topics_path\n";
            std::filesystem::create_directories(base_path);
        }

        // Create Topic base path
        std::filesystem::path topic_path = base_path / this->topic_name;
        std::filesystem::create_directories(topic_path);

        return topic_path;
    }

    void BasePublisher::cleanup_filesystem()
    {
        /*
        Close All Opened dba_buf and
        Remove All Related to dba_buf Topics
        */
        for (int i = 0; i < this->allocated_dma_bufs_fd.size(); i++)
        {
            close(this->allocated_dma_bufs_fd[i]);
            std::string filename = std::to_string(this->pid) + DPS_TOPIC_SEPARATOR +
                                   std::to_string(this->allocated_dma_bufs_fd[i]) + DPS_TOPIC_SEPARATOR +
                                   std::to_string(i) + DPS_TOPIC_SEPARATOR +
                                   std::to_string(this->queue_size);
            std::cout << "Remove " << this->topic_path / filename << "\n";
            std::filesystem::remove(this->topic_path / filename);
        }
    }

    std::vector<int> BasePublisher::allocate_dma_bufs()
    {
        std::vector<int> dma_bufs_fd(this->queue_size);

        for (int i = 0; i < queue_size; i++)
        {
            int fd = this->allocate(message_size);
            std::string filename = std::to_string(this->pid) + DPS_TOPIC_SEPARATOR +
                                   std::to_string(fd) + DPS_TOPIC_SEPARATOR +
                                   std::to_string(i) + DPS_TOPIC_SEPARATOR +
                                   std::to_string(this->queue_size);
            std::ofstream{this->topic_path / filename};
            dma_bufs_fd[i] = fd;
        }

        return dma_bufs_fd;
    }

    std::vector<std::unique_ptr<uint8_t>> BasePublisher::map_buffers()
    {
        std::vector<std::unique_ptr<uint8_t>> buffers(this->queue_size);

        return buffers;
    }

    int BasePublisher::allocate(size_t buffer_size)
    {
        int heap_fd = open(this->heap_name.c_str(), O_RDONLY | O_CLOEXEC);
        if (heap_fd < 0)
        {
            std::cerr << "open dma heap failed, heap: " << this->heap_name << "\n";
            raise(SIGSEGV);
        }

        struct dma_heap_allocation_data heap_data = {};
        heap_data.len = buffer_size;
        heap_data.fd_flags = O_RDWR | O_CLOEXEC;

        if (ioctl(heap_fd, DMA_HEAP_IOCTL_ALLOC, &heap_data) != 0)
        {
            std::cerr << "dma heap alloc failed, len: " << heap_data.len << "\n";
            close(heap_fd);
            raise(SIGSEGV);
        }
        close(heap_fd);

        char dma_buf_name[DMA_BUF_NAME_LEN - 1] = {};
        for (int i = 0; i < std::min(sizeof(dma_buf_name), this->topic_name.length()); i++)
        {
            dma_buf_name[i] = this->topic_name[i];
        }
        this->topic_name.c_str();
        if (ioctl(heap_data.fd, DMA_BUF_SET_NAME, dma_buf_name) != 0)
        {
            std::cerr << "dma buf rename failed, name: " << dma_buf_name << "\n";
        }

        return heap_data.fd;
    }
}
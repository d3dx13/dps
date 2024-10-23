#include "dps/core/publisher_base.h"

namespace dps
{
    PublisherBase::PublisherBase(std::string topic_name, size_t message_size,
                                 size_t queue_size, std::string heap_name)
        : topic_name(dps_common::simplify_topic_name(topic_name)),
          heap_name(heap_name)
    {
        dps_common::unleash(); // Remove linux limits

        this->init_topic_path(); // Create topic directory

        // Create DMABuffer array
        this->dma_buffers.resize(queue_size);
        for (int queue_id = 0; queue_id < queue_size; queue_id++)
        {
            this->dma_buffers[queue_id] = std::make_unique<dps::DMABuffer>(
                message_size, this->heap_name, this->topic_name);
        }

        // Create EventFD
        this->event = std::make_unique<dps::Event>();

        // Create PublisherInfo
        std::vector<uint8_t> publisher_info_serialized =
            this->generate_publisher_info_serialized(message_size, queue_size);

        // TODo remove
        std::cout << "Size: " << publisher_info_serialized.size() << "\n";
        for (int i = 0; i < publisher_info_serialized.size(); i++)
        {
            std::cout << ", " << (unsigned int)publisher_info_serialized[i];
        }
        std::cout << "\n";

        // TODO remove
        for (int queue_id = 0; queue_id < this->dma_buffers.size(); queue_id++)
        {
            std::cout << "fd: " << this->dma_buffers[queue_id].get()->fd();
            std::cout << ", size: " << this->dma_buffers[queue_id].get()->size()
                      << "\n";
        }
    }

    PublisherBase::~PublisherBase() {}

    void PublisherBase::init_topic_path()
    {
        // Create topic_path in filesystem
        this->topic_path = std::filesystem::path(DPS_BASE_PATH) / this->topic_name;
        if (!std::filesystem::exists(this->topic_path) or
            !std::filesystem::is_directory(this->topic_path))
        {
            std::filesystem::create_directories(this->topic_path);
        }
    }

    std::vector<uint8_t> PublisherBase::generate_publisher_info_serialized(size_t message_size,
                                                                           size_t queue_size)
    {
        // temporary object
        dps_msg::PublisherInfoT publisher_info = dps_msg::PublisherInfoT();

        // Resize All vectors
        publisher_info.dma_buff_fd.resize(queue_size);
        publisher_info.index_cycle_pool.resize(queue_size);
        publisher_info.timestamp_pool.resize(queue_size);
        publisher_info.msg_id_pool.resize(queue_size);

        // Fill all values
        publisher_info.queue_size = queue_size;
        publisher_info.message_size = message_size;
        publisher_info.pid = getpid();
        publisher_info.event_fd = this->event.get()->fd();
        for (int id = 0; id < this->dma_buffers.size(); id++)
        {
            publisher_info.dma_buff_fd[id] = this->dma_buffers[id].get()->fd();
        }
        publisher_info.last_index = -1;
        for (int id = 0; id < publisher_info.queue_size; id++)
        {
            publisher_info.index_cycle_pool[id] = -1;
        }

        // Create bytes buffer from temporary object
        flatbuffers::FlatBufferBuilder fbb;
        fbb.Finish(dps_msg::PublisherInfo::Pack(fbb, &publisher_info));

        return std::vector<uint8_t>(fbb.GetBufferPointer(),
                                    fbb.GetBufferPointer() + fbb.GetSize());
    }
} // namespace dps
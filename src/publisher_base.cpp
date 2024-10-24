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
        this->create_publisher_info(message_size, queue_size);
    }

    PublisherBase::~PublisherBase()
    {
        // Delete file publisher_info_file_path -> generate inotify event IN_DELETE
        std::cout << "Delete file publisher_info_file_path\n"; // TODO remove

        remove(this->publisher_info_file_path.c_str());

        this->publisher_info_buffer.reset();
        this->publisher_info = NULL;

        for (auto it = this->dma_buffers.begin(); it != this->dma_buffers.end(); it++)
        {
            std::cout << "cleanup buffer\n"; // TODO remove
            it->reset();
        }

        this->event.reset();
    }

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

    void PublisherBase::create_publisher_info(size_t message_size,
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

        // Fill dma_buf related to publisher_info
        this->publisher_info_buffer = std::make_unique<dps::DMABuffer>(
            fbb.GetSize(), this->heap_name, "info:" + this->topic_name);
        for (int i = 0; i < fbb.GetSize(); i++)
        {
            this->publisher_info_buffer.get()->buffer()[i] = fbb.GetBufferPointer()[i];
        }

        // Create PublisherInfo on buffer
        this->publisher_info = dps_msg::GetMutablePublisherInfo(this->publisher_info_buffer.get()->buffer());

        // Create publisher_info_file_path
        this->publisher_info_file_path = this->topic_path / ((DPS_TOPIC_SERVICE_CHARACER + std::to_string(this->publisher_info->pid())) +
                                                             (DPS_TOPIC_SERVICE_CHARACER + std::to_string(this->publisher_info_buffer.get()->fd())));

        // Create file publisher_info_file_path -> generate inotify event IN_CLOSE_WRITE
        int ublisher_info_file_fd;
        ublisher_info_file_fd = open(this->publisher_info_file_path.c_str(), O_WRONLY | O_CREAT, DPS_FILE_PERMISSION);
        close(ublisher_info_file_fd);
    }
} // namespace dps
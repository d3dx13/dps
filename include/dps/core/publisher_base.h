#include "dps/core/common.h"
#include "dps/core/dma_buffer.h"
#include "dps/core/event.h"
#include "dps/msg/publisher_info_generated.h"
#include "flatbuffers/flatbuffers.h"

namespace dps
{
    class PublisherBase
    {
    public:
        PublisherBase(std::string topic_name, size_t message_size, size_t queue_size, std::string heap_name);

        ~PublisherBase();

        void publish();

    protected:
        void init_topic_path();
        std::vector<uint8_t> generate_publisher_info_serialized(size_t message_size, size_t queue_size);

        const std::string topic_name;
        const std::string heap_name;

        std::filesystem::path topic_path;

        std::vector<std::unique_ptr<dps::DMABuffer>> dma_buffers;
        std::unique_ptr<dps::Event> event;
        // std::unique_ptr<dps::DMABuffer> event;
    };
}
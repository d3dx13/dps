#include "dps/core/common.h"
#include "dps/core/dma_buffer.h"

namespace dps {
    class PublisherBase {
        public:
            PublisherBase(std::string topic_name, size_t message_size, int queue_size, std::string heap_name);

            ~PublisherBase();

            void publish();
        
        protected:
            const std::string topic_name;
            const size_t message_size;
            const int queue_size;
            const std::string heap_name;

            std::filesystem::path topic_path;

            std::vector<std::unique_ptr<dps::DMABuffer>> dma_buffers;
    };
}
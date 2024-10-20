#include "dps/core/common.h"

namespace dps {
    class PublisherBase {
        public:
            PublisherBase(std::string topic_name, size_t message_size, int queue_size, std::string heap_name);

            ~PublisherBase();

            void publish();
    };
}
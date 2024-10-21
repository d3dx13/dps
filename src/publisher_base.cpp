#include "dps/core/common.h"
#include "dps/core/publisher_base.h"

namespace dps {
    PublisherBase::PublisherBase(std::string topic_name, size_t message_size, int queue_size, std::string heap_name) : 
            topic_name(dps_common::simplify_topic_name(topic_name)), message_size(message_size), queue_size(queue_size), heap_name(heap_name) {
        // Remove linux limits
        dps_common::unleash();
        
        // Create topic_path in filesystem
        this->topic_path = std::filesystem::path(DPS_BASE_PATH) / (DPS_TOPIC_SERVICE_CHARACER + std::string(DPS_TOPIC_PATH)) / this->topic_name;
        
        // Create DMABuffer array
        this->dma_buffers.resize(this->queue_size);
        for (int queue_id = 0; queue_id < this->queue_size; queue_id++) {
            this->dma_buffers[queue_id] = std::make_unique<dps::DMABuffer>(this->message_size, this->heap_name, this->topic_name, 
                this->topic_path / (DPS_TOPIC_SERVICE_CHARACER + std::string(DPS_PUB_PATH)), queue_id, this->queue_size);
        }

        // TODO add EventFD create

        // TODO remove
        for (int queue_id = 0; queue_id < this->dma_buffers.size(); queue_id++) {
            std::cout << "fd: " << this->dma_buffers[queue_id].get()->fd();
            std::cout << ", msg_id: " << this->dma_buffers[queue_id].get()->header()->msg_id << "\n";
        }
    }

    PublisherBase::~PublisherBase() {
        
    }
}
#include "dps/core/node.h"
#include "dma_proxy.h"


namespace dps {
    Node::Node() : Node(std::string("")){};
    Node::Node(std::string name){
        this->name = name;
        this->pid = getpid();

        struct timespec timestamp={0,0};
        clock_gettime(CLOCK_MONOTONIC, &timestamp);
        std::cout << "timestamp " << timestamp.tv_sec <<  " " << timestamp.tv_nsec << "\n";
        if (this->name.length() == 0) {
            this->name = std::string("unknown_") + std::to_string(timestamp.tv_sec) + std::string("_") + std::to_string(timestamp.tv_nsec);
        }

        std::cout << "New Node " << this->name <<  " " << this->pid << "\n";
    };

    Node::~Node(){
        std::cout << "Poka \n";
    }

    int Node::get_pid(){
        return this->pid;
    }
}

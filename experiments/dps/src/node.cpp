#include "dps/core/node.h"

namespace dps {
    Node::Node() : Node(std::string("")){};
    Node::Node(std::string name){
        for (int i = SIGHUP; i <= SIGTERM; i++){
            signal(i, Node::signalHandler);
        }

        this->name = name;
        this->pid = getpid();
        struct timespec timestamp={0,0};
        clock_gettime(CLOCK_MONOTONIC, &timestamp);
        if (this->name.length() == 0) {
            this->name = std::string("unknown_") + std::to_string(timestamp.tv_sec) + std::string("_") + std::to_string(timestamp.tv_nsec);
        }

        this->init_filesystem();
    };

    Node::~Node(){
        Node::signalHandler(0);
    }

    template<typename MessageT>
    Publisher<MessageT> Node::create_publisher(std::string path){
        return Publisher<MessageT>();
    }


    std::string Node::get_name(){
        return this->name;
    }
    
    int Node::get_pid(){
        return this->pid;
    }

    void Node::init_filesystem(){
        std::filesystem::path node_path = std::filesystem::path(DPS_BASE_PATH) / DPS_NODE_PATH;
        std::filesystem::path topics_path = std::filesystem::path(DPS_BASE_PATH) / DPS_TOPIC_PATH;

        if (!std::filesystem::exists(node_path) or !std::filesystem::is_directory(node_path)){
            std::cout << "Recreating node_path\n";
            std::filesystem::create_directories(node_path);
        }
        if (!std::filesystem::exists(topics_path) or !std::filesystem::is_directory(topics_path)){
            std::cout << "Recreating topics_path\n";
            std::filesystem::create_directories(topics_path);
        }

        // Create Node base path
        std::filesystem::create_directories(node_path / std::to_string(this->pid));

        // Save Node name
        std::ofstream name_file(node_path / std::to_string(this->pid) / "name");
        name_file << this->name << "\n";
        name_file.close();
    }

    void Node::signalHandler(int signum) {
        if (signum > 0) {
            std::cout << "Interrupt signal (" << signum << ") received, pid " << getpid() << "\n";
        } else {
            std::cout << "Calling Node Destructor, pid " << getpid() << "\n";
        }

        /*
        Remove PID's Node directory
        */
        std::filesystem::remove_all(std::filesystem::path(DPS_BASE_PATH) / DPS_NODE_PATH / std::to_string(getpid()));

        /*
        Remove All Related to PID Topics
        */
        std::string pid_start = std::to_string(getpid()) + DPS_TOPIC_SEPARATOR;
        for (auto const& dir_entry : std::filesystem::recursive_directory_iterator(std::filesystem::path(DPS_BASE_PATH) / DPS_TOPIC_PATH)) {
            if (!dir_entry.is_regular_file()){
                continue;
            }
            std::string filename_candidate = dir_entry.path().filename();
            if (pid_start.length() > filename_candidate.length()){
                continue;
            }
            if (filename_candidate.find(pid_start, 0) != 0){
                continue;
            }
            std::filesystem::remove_all(dir_entry.path());
        }
        
        exit(signum);
    }
}

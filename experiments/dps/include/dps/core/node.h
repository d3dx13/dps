#include "dps/core/config.h"

namespace dps {
    class Node {
        public:
            Node();
            Node(std::string name);

            ~Node();

            int get_pid();
        
        private:
            std::string name;
            int pid;
    };
}
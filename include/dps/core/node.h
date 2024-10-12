#include "dps/core/config.h"
#include "dps/core/publisher.hpp"

namespace dps {
    class Node {
        public:
            Node();
            Node(std::string name);

            ~Node();

            std::string get_name();
            int get_pid();

        protected:
            std::string name;
            int pid;

        private:
            void init_filesystem();

            static void signalHandler(int signum);
    };
}
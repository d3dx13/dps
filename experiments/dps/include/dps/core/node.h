#include "dps/core/config.h"
#include "dps/core/publisher.h"

namespace dps {
    class Node {
        public:
            Node();
            Node(std::string name);

            ~Node();

            std::string get_name();
            int get_pid();

        protected:
            template<typename MessageT>
            Publisher<MessageT> create_publisher(std::string path);

            std::string name;
            int pid;

        private:
            void init_filesystem();

            static void signalHandler(int signum);
    };
}
#include <exception>
#include <string>

namespace exceptions {
    class DmaBufferNotFound : public std::exception {
        std::string msg;
    public:
        explicit DmaBufferNotFound(const std::string &name) : msg(std::string("DMA Buffer Not Found : ") + name) {}

        virtual const char *what() const throw() {
            return msg.c_str();
        }
    };
}


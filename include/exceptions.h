#include <exception>
#include <string>

namespace exceptions {
    class DmaBufferNotFound : public std::exception {
        std::string msg;
    public:
        explicit DmaBufferNotFound(const std::string &name) : msg(std::string("DMA Buffer Not Found : ") +
                                                                  name) {}

        [[nodiscard]] const char *what() const noexcept override {
            return msg.c_str();
        }
    };

    class DmaBufferNoUserRights : public std::exception {
        std::string msg;
    public:
        explicit DmaBufferNoUserRights(const std::string &name) : msg(
                std::string("DMA Buffer No User Rights : ") + name + "\nTry:\n" + "sudo chown ${USER}:${USER} " +
                name) {}

        [[nodiscard]] const char *what() const noexcept override {
            return msg.c_str();
        }
    };
}


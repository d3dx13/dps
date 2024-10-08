#include "dps/core/config.h"

namespace dps {
    template<typename MessageT>
    class Publisher {
        public:
            Publisher(std::string path);
            ~Publisher();
    };
}
#include "dps/core/config.h"

namespace dps {
    template <typename MessageT>
    class Publisher {
        private:
            int queue;

        public:
            MessageT *msg;

            Publisher(int queue);

            void publish();
            void print();
    };

    template <typename MessageT> Publisher<MessageT>::Publisher(int queue)
    {
        this->queue = queue;

        // this->msg = new T{};

        /*
        this->message_pool = new T[this->queue];
        
        for (int i = 0; i < this->queue; i++){
            *(this->message_pool + i) = new T{};
        }
        */

        this->msg = new MessageT{};
    }

    template <typename MessageT> void Publisher<MessageT>::publish()
    {
        this->msg = new MessageT{};
    }

    template <typename MessageT> void Publisher<MessageT>::print()
    {
        std::cout << "this->queue = " << this->queue << "\n";
        std::cout << "this->msg->a = " << this->msg->a << "\n";
        std::cout << "this->msg->b = " << this->msg->b << "\n";
        std::cout << "this->msg->c = " << this->msg->c << "\n";
    }
}

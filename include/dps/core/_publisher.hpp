#include "dps/core/common.h"

namespace dps
{
    template <typename MessageT>
    class Publisher
    {
    private:
        int queue;

    public:
        std::unique_ptr<MessageT> msg;

        Publisher(int queue);

        void publish();
        void print();
    };

    template <typename MessageT>
    Publisher<MessageT>::Publisher(int queue)
    {
        this->queue = queue;

        // this->msg = std::make_unique<MessageT>();

        /*
        this->message_pool = std::make_unique<MessageT>[this->queue];

        for (int i = 0; i < this->queue; i++){
            *(this->message_pool + i) = std::make_unique<MessageT>();
        }
        */

        this->msg = std::make_unique<MessageT>();
    }

    template <typename MessageT>
    void Publisher<MessageT>::publish()
    {
        this->msg = std::make_unique<MessageT>();
    }

    template <typename MessageT>
    void Publisher<MessageT>::print()
    {
        std::cout << "this->queue = " << this->queue << "\n";
        std::cout << "this->msg->a = " << this->msg->a << "\n";
        std::cout << "this->msg->b = " << this->msg->b << "\n";
        std::cout << "this->msg->c = " << this->msg->c << "\n";
    }
}

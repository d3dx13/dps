#include <iostream>
#include <unistd.h>

using namespace std;

struct test_msg {
    int a;
    int b;
    int c;
    int d[];
};

template <typename MessageT> class Publisher {
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
    cout << "this->queue = " << this->queue << "\n";
    cout << "this->msg->a = " << this->msg->a << "\n";
    cout << "this->msg->b = " << this->msg->b << "\n";
    cout << "this->msg->c = " << this->msg->c << "\n";
}


int main(int argc, char *argv[])
{
    Publisher<test_msg> pub(10);

    pub.print();

    pub.msg->a = 12312;
    pub.msg->b = 111;
    pub.msg->c = 44444;
    cout << "size " << sizeof(pub.msg) << "\n";

    pub.print();

    pub.publish();
    pub.print();

    // cout << this->message_pool[0]
    
    return 0;
}
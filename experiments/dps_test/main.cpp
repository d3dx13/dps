#include "dps/dps.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>

using namespace std;

struct test_msg {
    int a;
    int b;
    int c;
};

class SimpleNode : public dps::Node {
    public:
        SimpleNode() : dps::Node("SimpleNode") {
            this->base_pub = std::make_unique<dps::BasePublisher>("////image", 1000 * 1000 * 1000, 5, "/dev/dma_heap/system");
            cout << "\n" << this->base_pub->heap_name << " : " << this->base_pub->topic_name << " : " 
                << this->base_pub->queue_size << " : " << this->base_pub->message_size << "\n";
        }
    private:
        std::unique_ptr<dps::BasePublisher> base_pub;
};

int main(int argc, char *argv[])
{
    int temp;

    SimpleNode node = SimpleNode();

    cout << "pid " << node.get_pid() << "\n";
    
    system("tree -ahp /dev/shm/");

    cin >> temp;

    // raise(SIGSEGV);
    
    return 0;
}
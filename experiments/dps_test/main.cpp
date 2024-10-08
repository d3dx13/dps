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
        SimpleNode() : dps::Node("SimpleNode"){
            dps::Publisher<int> pub = this->create_publisher<int>("test");
        }
};

int main(int argc, char *argv[])
{
    int temp;

    SimpleNode node = SimpleNode();

    cout << "pid " << node.get_pid() << "\n";
    
    system("tree -a /dev/shm/");

    cin >> temp;

    // raise(SIGSEGV);
    
    return 0;
}
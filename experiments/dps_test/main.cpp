#include "dps/core.h"
#include <iostream>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[])
{
    int temp;

    dps::Node node = dps::Node("testawd");

    cout << "pid " << node.get_pid() << "\n";
    int heap_fd = dmabuf_heap_open();
    cout << "heap_fd = " << heap_fd << "\n";
    for (int i = 0; i < 100; i++){
        dmabuf_heap_alloc(heap_fd, "test", 1024 * 1024 * 100);
    }
    cin >> temp;
    return 0;
}
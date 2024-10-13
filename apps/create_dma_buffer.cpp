#include "dps/dps.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>

using namespace std;

int main(int argc, char *argv[])
{
    int temp;

    dps::DMABuffer dma;
    dma.allocate(1231, "system", "some stupid buffer");
    cout << "Size: " << dma.get_size() << "\n";
    dma.map();
    for (int i = 0; i < 1231; i++){
        dma.buffer[i] = (uint8_t)(200 - i);
        dma.header->msg_id += 1;
    }

    cout << "pid " << getpid() << "\n";
    cout << "fd " << dma.get_fd() << "\n";
    
    system(("cat /proc/" + to_string(getpid()) + "/fdinfo/*").c_str());

    cin >> temp;
    
    return 0;
}
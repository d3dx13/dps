#include "dps/dps.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>

using namespace std;

int main(int argc, char *argv[])
{
    int temp;

    dps::DMABuffer dma(64 - 1, "system", "some stupid buffer");
    cout << "Size: " << dma.size() << "\n";
    cout << "pid " << getpid() << "\n";
    cout << "fd " << dma.fd() << "\n";
    cout << "\n";
    for (int i = 0; i < dma.size(); i++){
        dma.buffer()[i] = (uint8_t)(200 - i);
        dma.header()->msg_id += 1;
    }
    for (int i = 0; i < dma.size(); i++){
        cout << (int) dma.buffer()[i] << ", ";
    }

    // dma.~DMABuffer();
    
    system(("cat /proc/" + to_string(getpid()) + "/fdinfo/*").c_str());

    cin >> temp;
    
    return 0;
}
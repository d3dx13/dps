#include "dps/dps.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>

using namespace std;

int main(int argc, char *argv[])
{
    int temp;

    dps::DMABuffer dma(64 - 1, "system", "some stupid buffer", "/dev/shm/test_file"); // , "/dev/shm/test_file"
    system("tree -a /dev/shm/");
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

    cin >> temp;

    system("tree -a /dev/shm/");
    dma.~DMABuffer();
    system("tree -a /dev/shm/");
    
    // system(("cat /proc/" + to_string(getpid()) + "/fdinfo/*").c_str());
    
    return 0;
}
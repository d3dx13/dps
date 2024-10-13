#include "dps/dps.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>

using namespace std;

int main(int argc, char *argv[])
{
    dps::DMABuffer dma;

    int temp;
    int pid;
    int fd;

    cout << "pid " << getpid() << "\n";
    cin >> pid;
    cin >> fd;

    dma.connect(pid, fd);
    cout << "Size: " << dma.get_size() << "\n";
    dma.map();
    cout << "dma.header->msg_id: " << dma.header->msg_id << "\n";
    for (int i = 0; i < dma.get_size(); i++){
        cout << (int) dma.buffer[i] << ", ";
    }
    cout << "\n";
    
    
    system(("cat /proc/" + to_string(getpid()) + "/fdinfo/*").c_str());

    cin >> temp;

    // raise(SIGSEGV);
    
    return 0;
}
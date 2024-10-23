#include "dps/dps.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>

using namespace std;

int main(int argc, char *argv[])
{
    int temp;
    int pid;
    int fd;

    cout << "pid " << getpid() << "\n";
    cin >> pid;
    cin >> fd;

    dps::DMABuffer dma(pid, fd);
    cout << "Size: " << dma.size() << "\n";
    for (int i = 0; i < dma.size(); i++)
    {
        cout << (int)dma.buffer()[i] << ", ";
    }
    cout << "\n";

    system(("cat /proc/" + to_string(getpid()) + "/fdinfo/*").c_str());

    cin >> temp;

    // raise(SIGSEGV);

    return 0;
}
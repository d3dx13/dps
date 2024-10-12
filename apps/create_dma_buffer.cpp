#include "dps/dps.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>

using namespace std;

int main(int argc, char *argv[])
{
    int temp;

    dps::DMABuffer dma;
    dma.allocate(1, "system", "some stupid buffer");

    cout << "pid " << getpid() << "\n";
    cout << "fd " << dma.get_fd() << "\n";
    
    system(("cat /proc/" + to_string(getpid()) + "/fdinfo/*").c_str());

    cin >> temp;
    
    return 0;
}
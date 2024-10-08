#include "dps/dps.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>

using namespace std;

int main(int argc, char *argv[])
{
    int temp;

    dps::Node node = dps::Node("testawd");

    cout << "pid " << node.get_pid() << "\n";
    
    system("tree -a /dev/shm/");

    cin >> temp;

    // raise(SIGSEGV);
    
    return 0;
}
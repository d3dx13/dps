#include "dps/dps.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>

using namespace std;

int main(int argc, char *argv[])
{
    int temp;
    cout << "pid: " << getpid() << "\n";
    system("rm -rf /dev/shm/.dps/*");
    system("tree -a /dev/shm/");

    dps::PublisherBase pub("////test/\\/\\/\\pu\\b\\///lol///", 1000 * 1000 * 1000, 4, "system");
    system("tree -a /dev/shm/");

    cin >> temp;

    system("tree -a /dev/shm/");
    pub.~PublisherBase();
    system("tree -a /dev/shm/");
    
    return 0;
}
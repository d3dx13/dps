#include "dps/dps.h"
#include <iostream>
#include <signal.h>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[])
{
    int temp;
    cout << "pid: " << getpid() << "\n";
    system("rm -rf /dev/shm/.dps/test/pub/lol/*");
    system("tree -a /dev/shm/");

    dps::PublisherBase pub("////test/\\/\\/\\pu\\b\\///lol///",
                           1920 * 1080, 4, "system");
    system("tree -a /dev/shm/");

    cin >> temp;

    system("tree -a /dev/shm/");
    pub.~PublisherBase();
    system("tree -a /dev/shm/");

    return 0;
}
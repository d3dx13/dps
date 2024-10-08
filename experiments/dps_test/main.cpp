#include "dps/dps.h"
#include <iostream>
#include <unistd.h>

using namespace std;

int main(int argc, char *argv[])
{
    int temp;

    dps::Node node = dps::Node("testawd");

    cout << "pid " << node.get_pid() << "\n";
    
    return 0;
}
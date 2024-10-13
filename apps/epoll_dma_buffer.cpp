#include "dps/dps.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>

#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

using namespace std;

int main(int argc, char *argv[])
{
    dps::DMABuffer dma1;
    dps::DMABuffer dma2;

    int temp;
    int pid = 24873;

    cout << "pid " << getpid() << "\n";

    dma1.connect(pid, 9);
    dma2.connect(pid, 10);
    int fd1 = dma1.get_fd();
    int fd2 = dma2.get_fd();

    int epfd;
    struct epoll_event ev;
    struct epoll_event evlist[1024];
    
    system(("cat /proc/" + to_string(getpid()) + "/fdinfo/*").c_str());

    epfd = epoll_create1(0);
    ev.events = EPOLLET;
    ev.data.fd = fd1;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd1, &ev);
    ev.data.fd = fd2;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd2, &ev);

    int nfds;

    while (true)
    {
        nfds = epoll_wait(epfd, evlist, 1024, -1);
        for (int i = 0; i < nfds; i++)
        {
            cout << evlist[i].data.fd << " - " << evlist[i].events << "\n";
        }
    }

    cin >> temp;

    // raise(SIGSEGV);
    
    return 0;
}
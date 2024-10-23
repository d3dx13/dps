
#include <stdio.h>
#include <sys/stat.h>
#include <sys/inotify.h>
#include <sys/epoll.h>
#include "dps/dps.h"
#include <iostream>
#include <signal.h>
#include <unistd.h>

using namespace std;

#define EVENT_SIZE sizeof(struct inotify_event)

vector<string> paths = {
    "/dev/shm/.dps/test/pub/lol",
};

unsigned int mask = IN_CLOSE_WRITE | IN_DELETE | IN_DELETE_SELF;

int inotify_fd = -1;

int main(int argc, char *argv[])
{
start:
    close(inotify_fd);

    vector<int> fds;
    inotify_fd = inotify_init1(IN_NONBLOCK);

    for (auto path : paths)
    {
        struct stat sb;
        if (stat(path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
        {
            printf("YES %s\n", path.c_str());
        }
        else
        {
            // printf("NO %s\n", path.c_str());
            usleep(100000);
            goto start;
        }
        fds.push_back(inotify_add_watch(inotify_fd, path.c_str(), mask));
    }

    printf("inotify_fd = %d\n", inotify_fd);

    while (true)
    {
        int n;
        ioctl(inotify_fd, FIONREAD, &n);
        if (n == 0)
        {
            usleep(1000);
            continue;
        }
        int length, i = 0, wd;
        char *buffer = new char[n];

        i = 0;
        /* высчитываем размер файлового дескриптора*/
        length = read(inotify_fd, buffer, n);
        if (length < 0)
        {
            perror("read");
            delete[] buffer;
            exit(EXIT_FAILURE);
        }

        printf("length %i\n", length);
        while (i < length)
        {
            struct inotify_event *event = (struct inotify_event *)&buffer[i];
            printf("mask %d\n", event->mask);
            printf("name %s\n", event->name);
            if (event->mask & IN_DELETE_SELF)
            {
                printf("The file was IN_DELETE_SELF with WD %d\n", event->wd);
                delete[] buffer;
                goto start;
            }
            if (event->len)
            {
                if (event->mask == IN_CLOSE_WRITE)
                {
                    printf("The file %s was IN_CLOSE_WRITE with WD %d\n", event->name, event->wd);
                }
                else if (event->mask == IN_DELETE)
                {
                    printf("The file %s was IN_DELETE with WD %d\n", event->name, event->wd);
                }
                i += EVENT_SIZE + event->len;
            }
            else
            {
                i += EVENT_SIZE;
                continue;
            }
        }
        delete[] buffer;
    }

    return 0;
}

#include <stdio.h>
#include <sys/stat.h>
#include <sys/inotify.h>
#include <sys/epoll.h>
#include "dps/dps.h"
#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <dirent.h>

#include <unordered_map>
#include "dps/msg/publisher_info_generated.h"

using namespace std;

#define EVENT_SIZE sizeof(struct inotify_event)
#define USLEEP 1000

vector<string> paths = {
    "/dev/shm/.dps/test/pub/lol",
};

unsigned int mask = IN_CLOSE_WRITE | IN_DELETE | IN_DELETE_SELF;

struct dirent *de;
struct stat sb;
int watch_fd = -1;
int inotify_fd = -1;

std::unique_ptr<dps::DMABuffer> sub_buffer;
const dps_msg::PublisherInfo *sub_info;
unordered_map<int, std::unique_ptr<dps::DMABuffer>> sub_map_dma_buff;

std::vector<std::string> split(const std::string &s, char seperator)
{
    std::vector<std::string> output;

    std::string::size_type prev_pos = 0, pos = 0;

    while ((pos = s.find(seperator, pos)) != std::string::npos)
    {
        std::string substring(s.substr(prev_pos, pos - prev_pos));

        output.push_back(substring);

        prev_pos = ++pos;
    }

    output.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word

    return output;
}

void open_dma_buffer(char *file_info)
{
    string file_info_str = string(file_info);
    vector<string> file_info_values = split(file_info, '\\');
    int pid = stoi(file_info_values[1]);
    int fd = stoi(file_info_values[2]);
    sub_buffer = make_unique<dps::DMABuffer>(pid, fd);
    sub_info = dps_msg::GetPublisherInfo(sub_buffer.get()->buffer());
}

int main(int argc, char *argv[])
{
    printf("inotify_fd = %d\n", inotify_fd);

start:

    printf("\e[1;1H\e[2J");
    fflush(stdout);

    if (inotify_fd > 0)
    {
        close(inotify_fd);
        inotify_fd = -1;
    }
    sub_map_dma_buff.clear();
    sub_buffer.reset();
    sub_info = NULL;

    for (string path : paths)
    {
        if (stat(path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
        {
            printf("YES %s\n", path.c_str());
            inotify_fd = inotify_init1(IN_NONBLOCK);
            watch_fd = inotify_add_watch(inotify_fd, path.c_str(), mask);

            DIR *dr = opendir(path.c_str());
            while ((de = readdir(dr)) != NULL)
            {
                if (!(de->d_name[0] == '.' || de->d_name[1] == '.'))
                {
                    printf("New File %s\n", de->d_name);
                    if (sub_info == NULL)
                    {
                        open_dma_buffer(de->d_name);
                    }
                }
            }
            closedir(dr);
        }
        else
        {
            usleep(USLEEP);
            goto start;
        }
    }

    while (true)
    {
        if (sub_info != NULL)
        {
            printf("\e[1;1H\e[2J");
            cout << "my_pid: " << getpid();
            cout << "\nsub_buffer_fd: " << sub_buffer.get()->fd();
            cout << "\npid: " << sub_info->pid();
            cout << "\nlast_index: " << sub_info->last_index();
            cout << "\nindex_cycle_pool: ";
            for (int i = 0; i < sub_info->index_cycle_pool()->size(); i++)
            {
                if (i > 0)
                    cout << ", ";
                cout << sub_info->index_cycle_pool()->Get(i);
            }
            cout << "\nmsg_id_pool: ";
            sub_info->msg_id_pool();
            for (int i = 0; i < sub_info->msg_id_pool()->size(); i++)
            {
                if (i > 0)
                    cout << ", ";
                cout << sub_info->msg_id_pool()->Get(i);
            }
            cout << "\ndma_buff_fd: ";
            for (int i = 0; i < sub_info->dma_buff_fd()->size(); i++)
            {
                if (i > 0)
                    cout << ", ";
                cout << sub_info->dma_buff_fd()->Get(i);
            }
            fflush(stdout);
        }

        int n;
        ioctl(inotify_fd, FIONREAD, &n);
        if (n == 0)
        {
            usleep(USLEEP);
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
            // printf("mask %d\n", event->mask);
            // printf("name %s\n", event->name);
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
                    if (sub_info == NULL)
                    {
                        open_dma_buffer(event->name);
                    }
                }
                else if (event->mask == IN_DELETE)
                {
                    printf("The file %s was IN_DELETE with WD %d\n", event->name, event->wd);
                    delete[] buffer;
                    goto start;
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
#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <signal.h>
#include <vector>

#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <algorithm>

#include <fcntl.h>
#include <linux/dma-buf.h>
#include <linux/dma-heap.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include <sys/resource.h>
#include <sys/syscall.h>
#include <sys/eventfd.h>

#define RLIMIT_NOFILE_MAX 1048576
#define DMA_HEAP_PATH "/dev/dma_heap/"

#define DPS_BASE_PATH "/dev/shm/.dps/"
#define DPS_SUB_PATH "sub"
#define DPS_PUB_PATH "pub"

#define DPS_TOPIC_SERVICE_CHARACER '\\'

namespace dps_common
{
    // trim from start (in place)
    static void ltrim(std::string &s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
                                        { return !std::isspace(ch); }));
    }

    // trim from end (in place)
    static void rtrim(std::string &s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
                             { return !std::isspace(ch); })
                    .base(),
                s.end());
    }

    // trim from both ends (in place)
    static void trim(std::string &s)
    {
        rtrim(s);
        ltrim(s);
    }

    static std::string simplify_topic_name(std::string topic_name)
    {
        std::string topic_name_new;
        bool once_slash = false;
        for (int index = 0; index < topic_name.length(); index++)
        {
            if (topic_name[index] == DPS_TOPIC_SERVICE_CHARACER)
            {
                continue;
            }
            if (topic_name[index] == '/')
            {
                if (!once_slash && topic_name_new.length() > 0)
                {
                    topic_name_new += topic_name[index];
                }
                once_slash = true;
            }
            else
            {
                topic_name_new += topic_name[index];
                once_slash = false;
            }
        }
        if (topic_name_new[topic_name_new.length() - 1] == '/')
        {
            topic_name_new.pop_back();
        }
        return topic_name_new;
    }

    static void unleash()
    {
        // Remove limit on opened file descriptors
        rlimit rlimit_nofile{RLIMIT_NOFILE_MAX, RLIMIT_NOFILE_MAX};
        setrlimit(RLIMIT_NOFILE, &rlimit_nofile);
    }
}

#endif
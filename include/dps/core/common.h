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

#include <sys/syscall.h>

#define PAGE_SIZE 4096
#define DMA_HEAP_PATH "/dev/dma_heap/"

#define DPS_BASE_PATH "/dev/shm/.dps"
#define DPS_TOPIC_PATH "topic"
#define DPS_NODE_PATH "node"

#define DPS_TOPIC_SEPARATOR ':'

namespace dps_common {
    // trim from start (in place)
    static void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
    }

    // trim from end (in place)
    static void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    // trim from both ends (in place)
    static void trim(std::string &s) {
        rtrim(s);
        ltrim(s);
    }

    static std::string simplify_topic_name(std::string topic_name) {
        int start_index;
        for (start_index = 0; start_index < topic_name.length(); start_index++) {
            if (topic_name[start_index] != '/') {
                break;
            }
        }
        return topic_name.substr(start_index);
    }
}

#endif
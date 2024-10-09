#include <stdlib.h>
#include <signal.h>
#include <vector>

#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

#include <fcntl.h>
#include <linux/dma-buf.h>
#include <linux/dma-heap.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#define DPS_BASE_PATH "/dev/shm/.dps"
#define DPS_TOPIC_PATH "topic"
#define DPS_NODE_PATH "node"

#define DPS_TOPIC_SEPARATOR ':'

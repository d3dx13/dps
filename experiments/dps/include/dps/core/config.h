#include <stdlib.h>
#include <signal.h>

#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

#define DPS_BASE_PATH "/dev/shm/.dps"
#define DPS_TOPIC_PATH "topic"
#define DPS_NODE_PATH "node"

#define DPS_TOPIC_SEPARATOR ':'

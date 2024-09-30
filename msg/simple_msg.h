#include "ips.h"

enum mode {
    FIRST = 1,
    SECOND = 2,
    THIRD = 4
};

struct simple_msg {
    uint16 a;
    uint32 b;
    uint8 image;
    float32 fv;
    mode mm;
    byte name[8];
}; // compressed

struct simple_msg_dynamic {
    uint16 a;
    uint32 b;
    uint8 image;
    float32 fv;
    mode mm;
    byte *name;
}; // compressed

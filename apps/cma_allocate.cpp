#include <iostream>
#include "ips.h"

int main() {
    // Allocator alloc(Allocator::memory::RESERVED);
    Allocator alloc_alt;

    std::string out_str;
    std::cout << "Wait Key " << getpid();
    std::cin >> out_str;
    return 0;
}

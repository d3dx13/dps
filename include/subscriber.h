#pragma once

#include <string>


class Subscriber {
public:
    explicit Subscriber(std::string name);
    bool get();
    bool get_last();
};
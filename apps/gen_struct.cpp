#include <iostream>
#include <string>
#include <unordered_map>
#include <bitset>
#include <climits>
#include <vector>
#include <type_traits>
#include <cstring>
#include <simple_msg.h>

#define IS_INTEGRAL(T) typename std::enable_if<std::is_integral<T>::value>::type * = 0

template <class T>
std::string integral_to_binary_string(T byte, IS_INTEGRAL(T))
{
    std::bitset<sizeof(T) * CHAR_BIT> bs(byte);
    return bs.to_string();
}

int main()
{
    unsigned char buffer[32] = {0};

    simple_msg *msg = new (buffer) simple_msg;
    std::cout << "a = " << msg->a << "\n";
    std::cout << "b = " << msg->b << "\n";
    std::cout << "image = " << msg->image << "\n";
    std::cout << "fv = " << msg->fv << "\n";
    std::cout << "mm = " << msg->mm << "\n";
    for (int i = 0; i < sizeof(buffer) / sizeof(buffer[0]); i++)
    {
        std::cout << i << ":" << integral_to_binary_string(buffer[i]) << "\n";
    }

    // size_t dataSize = std::size(msg->a);
    // std::cout << "Length of array = " << dataSize << std::endl;

    msg->a = 23123;
    msg->b = 444413124;
    msg->image = 127;
    msg->fv = 321.1231;
    msg->mm = mode::FIRST;

    std::cout << "a = " << msg->a << "\n";
    std::cout << "b = " << msg->b << "\n";
    std::cout << "image = " << msg->image << "\n";
    std::cout << "fv = " << msg->fv << "\n";
    std::cout << "mm = " << msg->mm << "\n";
    for (int i = 0; i < sizeof(buffer) / sizeof(buffer[0]); i++)
    {
        std::cout << i << ":" << integral_to_binary_string(buffer[i]) << "\n";
    }

    std::cout << sizeof(simple_msg) << "\n";

    return 0;
}

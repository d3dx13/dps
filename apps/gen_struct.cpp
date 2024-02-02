#include <iostream>
#include <string>
#include <bitset>
#include <climits>
#include <type_traits>

struct message {
    std::uint16_t *a;
    std::uint32_t *b;
    std::uint8_t *image;
    float fv;
};

#define IS_INTEGRAL(T) typename std::enable_if< std::is_integral<T>::value >::type* = 0

template<class T>
std::string integral_to_binary_string(T byte, IS_INTEGRAL(T))
{
    std::bitset<sizeof(T) * CHAR_BIT> bs(byte);
    return bs.to_string();
}

int main() {
    char buffer[32];
    for (char ch: buffer) {
        std::cout << "ch: " << (int) ch << "\n";
    }
    std::cout << "\n";

    // std::uint8_t &b = *std::any_cast<std::uint8_t>(&umap["i8"]);

    //for (auto &it: umap) {
    //    std::cout << it.first << " : " << std::get<std::uint8_t>(it.second) << "\n";
    //}

    message msg{};
    msg.a = new(buffer + 0) std::uint16_t[2];
    msg.b = new(buffer + 8) std::uint32_t[2];
    msg.image = new(buffer + 16) std::uint8_t[2];

    // auto *pA = static_cast<message *>(static_cast<void *>(&buffer[2]));

    std::cout << "a 0 = " << msg.a[0] << "\n";
    std::cout << "b 0 = " << msg.b[0] << "\n";
    std::cout << "a 1 = " << msg.a[1] << "\n";
    std::cout << "b 1 = " << msg.b[1] << "\n";
    std::cout << "bt 0 = " << (unsigned int) msg.image[0] << "\n";
    std::cout << "bt 1 = " << (unsigned int) msg.image[1] << "\n";
    std::cout << "size: " << sizeof(message) << "\n";
    for (char ch: buffer) {
        std::cout << "ch: " << integral_to_binary_string(ch) << "\n";
    }
    std::cout << "\n";

    msg.a[0] = 12312;
    msg.a[1] = 2222;
    msg.b[0] = 1312312312;
    msg.b[1] = 222322223;
    std::cout << "a 0 = " << msg.a[0] << "\n";
    std::cout << "b 0 = " << msg.b[0] << "\n";
    std::cout << "a 1 = " << msg.a[1] << "\n";
    std::cout << "b 1 = " << msg.b[1] << "\n";
    std::cout << "bt 0 = " << (unsigned int) msg.image[0] << "\n";
    std::cout << "bt 1 = " << (unsigned int) msg.image[1] << "\n";
    std::cout << "size: " << sizeof(message) << "\n";
    for (char ch: buffer) {
        std::cout << "ch: " << integral_to_binary_string(ch) << "\n";
    }
    std::cout << "\n";

    return 0;
}

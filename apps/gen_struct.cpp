#include <iostream>
#include <string>
#include <unordered_map>
#include <bitset>
#include <climits>
#include <vector>
#include <type_traits>
#include <variant>
#include <cstring>
#include <any>

struct message {
    std::uint16_t *a;
    std::uint32_t *b;
    std::uint8_t *image;
    float fv;
};

#define IS_INTEGRAL(T) typename std::enable_if< std::is_integral<T>::value >::type* = 0

template<class T>
std::string integral_to_binary_string(T byte, IS_INTEGRAL(T)) {
    std::bitset<sizeof(T) * CHAR_BIT> bs(byte);
    return bs.to_string();
}

std::unordered_map<std::string, std::any> generate_map(
        std::vector<std::vector<std::string>> info,
        unsigned char * buffer,
        unsigned long offset
        ) {
    std::unordered_map<std::string, std::any> result;
    for (std::vector<std::string> &field: info) {
        result[field.at(1)] = new (buffer + offset) std::uint8_t[1];
    }
    return result;
}

int main() {
    unsigned char buffer [32] = {0};

    std::vector<std::vector<std::string>> info;
    std::vector<std::string> tmp;
    tmp.emplace_back("uint8"); // type
    tmp.emplace_back("a"); // name
    tmp.emplace_back("4,6"); // shape
    info.push_back(tmp);
    auto gen_map = generate_map(info, buffer, 0);

    for (auto ch: buffer) {
        std::cout << "ch : " << (unsigned int) ch << "\n";
    }
    for (auto value: gen_map) {
        auto casted = std::any_cast<std::uint8_t*>(value.second);
        std::cout << value.first << " : " << (unsigned int) casted[0] << "\n";
    }
    std::cout << "\n";

    gen_map["a"] = 42;

    for (auto ch: buffer) {
        std::cout << "ch : " << (unsigned int) ch << "\n";
    }
    for (auto value: gen_map) {
        auto casted = std::any_cast<std::uint8_t>(value.second);
        std::cout << value.first << " : " << (unsigned int) casted << "\n";
    }
    std::cout << "\n";

    return 0;
}

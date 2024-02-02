#include <iostream>
#include <unordered_map>
#include <variant>
#include <cstring>
#include <any>

struct message {
    std::uint16_t a[2];
    std::uint32_t b[2];
};

int main() {
    std::unordered_map<std::string, std::variant<std::uint8_t, double, float, std::string>> umap;
    umap["a"] = (double) 1231.1231;
    umap["b"] = (float) 455.555;
    umap["s"] = std::string("123123121321");
    umap["i8"] = (std::uint8_t) 23;

    char buffer[16] = {0};
    for (char ch : buffer){
        std::cout << "ch: " << (int)ch << "\n";
    }
    std::cout << "\n";

    // std::uint8_t &b = *std::any_cast<std::uint8_t>(&umap["i8"]);

    //for (auto &it: umap) {
    //    std::cout << it.first << " : " << std::get<std::uint8_t>(it.second) << "\n";
    //}

    auto *pA = static_cast<message *>(static_cast<void*>(&buffer[2]));

    std::cout << "a 0 = " << pA->a[0] << "\n";
    std::cout << "b 0 = " << pA->b[0] << "\n";
    std::cout << "a 1 = " << pA->a[1] << "\n";
    std::cout << "b 1 = " << pA->b[1] << "\n";
    std::cout << "size: " << sizeof(pA) << "\n";
    for (char ch : buffer){
        std::cout << "ch: " << (int)ch << "\n";
    }
    std::cout << "\n";

    pA->a[0] = 12312;
    pA->a[1] = 2222;
    pA->b[0] = 1312312312;
    pA->b[1] = 222322223;
    std::cout << "a 0 = " << pA->a[0] << "\n";
    std::cout << "b 0 = " << pA->b[0] << "\n";
    std::cout << "a 1 = " << pA->a[1] << "\n";
    std::cout << "b 1 = " << pA->b[1] << "\n";
    std::cout << "size: " << sizeof(pA) << "\n";
    for (char ch : buffer){
        std::cout << "ch: " << (unsigned int)ch << "\n";
    }
    std::cout << "\n";

    return 0;
}

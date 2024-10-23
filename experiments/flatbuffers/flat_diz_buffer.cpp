#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <fstream>

#include "image_generated.h"
 
void print(char *data, int length) {
    std::cout << "length: " << length << ", data:\n";
    for (int i = 0; i < length; i++) {
        std::cout << (int) (unsigned char) data[i] << ", ";
    }
    std::cout << "\n";
}

int main(int argc, char *argv[])
{
    std::ifstream infile;
    infile.open("buffer.bin", std::ios::binary | std::ios::in);
    infile.seekg(0,std::ios::end);
    int length = infile.tellg();
    infile.seekg(0,std::ios::beg);
    char *data = new char[length];
    infile.read(data, length);
    infile.close();
    
    std::cout << "\n\n BEGIN \n";
    print(data, 256);

    auto image = ImageTools::GetMutableImage(data);
    std::cout << "raw_size: " << image->raw()->size() << "\n";
    std::cout << "raw:\n";
    for (int i = 0; i < image->raw()->size(); i++) {
        std::cout << (int) image->raw()->Get(i) << ", ";
    }
    std::cout << "\n";

    std::cout << "\n\n CORRUPT \n";
    for (int i = 32; i < 70; i++) {
        data[i] = 13;
    }

    print(data, 256);
    std::cout << "raw_size: " << image->raw()->size() << "\n";
    std::cout << "raw:\n";
    for (int i = 0; i < image->raw()->size(); i++) {
        std::cout << (int) image->raw()->Get(i) << ", ";
    }
    std::cout << "\n";

    // std::cout << "\nbuffer_raw_begin\n";
    // std::cout << (int) image->raw() << "\n";

    std::cout << "\n\n REPLACE \n";
    for (int i = 0; i < image->mutable_raw()->size(); i++) {
        image->mutable_raw()->Mutate(i, 42);
    }
    image->mutable_size()->mutate_depth(1);
    image->mutable_size()->mutate_width(320);
    image->mutable_size()->mutate_height(240);
    
    print(data, 256);
    std::cout << "raw_size: " << image->raw()->size() << "\n";
    std::cout << "raw:\n";
    for (int i = 0; i < image->raw()->size(); i++) {
        std::cout << (int) image->raw()->Get(i) << ", ";
    }
    std::cout << "\n";

    return 0;
}
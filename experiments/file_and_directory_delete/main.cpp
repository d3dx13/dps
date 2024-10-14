#include <iostream>
#include <unistd.h>
#include <vector>
#include <fstream>
#include <filesystem>

// dma_buf
#include <stdlib.h>
#include <signal.h>
#include <memory>
#include <queue>

using namespace std;



int main(int argc, char *argv[])
{
    system("rm -rf /dev/shm/*");
    system("rm -rf /dev/shm/.*");
    system("tree -a /dev/shm/");

    cout << "Create Directory\n";
    std::filesystem::path file_path = std::filesystem::path("/dev/shm/temp/.test1");
    std::filesystem::create_directories(file_path.parent_path());
    system("tree -a /dev/shm/");

    cout << "Create File 1\n";
    std::ofstream {file_path};
    system("tree -a /dev/shm/");

    cout << "Create File 2\n";
    std::filesystem::path file_path2 = std::filesystem::path("/dev/shm/.temp");
    std::ofstream {file_path2};
    system("tree -a /dev/shm/");

    cout << "Delete File 2\n";
    std::filesystem::remove(file_path2);
    system("tree -a /dev/shm/");

    cout << "Delete File 1\n";
    std::filesystem::remove(file_path);
    system("tree -a /dev/shm/");

    return 0;
}
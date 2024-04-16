#include <iostream>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/mman.h>

using namespace cv;

int borrow_fd_from_pid(int pid, int fd)
{
    int pid_fd = syscall(SYS_pidfd_open, pid, 0);   // PIDFD_NONBLOCK
    return syscall(SYS_pidfd_getfd, pid_fd, fd, 0); // PTRACE_MODE_ATTACH_REALCREDS
}

int main()
{
    int pub_pid = 7740;
    int pub_fd = 44;
    int target_fd = borrow_fd_from_pid(pub_pid, pub_fd);

    int frame_size = 320 * 240 * 3;
    unsigned char* dataBuffer = static_cast<unsigned char*>(mmap(NULL, frame_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, target_fd, 0u));
    Mat image(Size(320, 240), CV_8UC3, dataBuffer);

    while (true)
    {
        cv::imshow("array", image);
        cv::waitKey(1);
    }

    return 0;
}

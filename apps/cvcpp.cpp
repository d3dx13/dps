#include <iostream>
#include <opencv2/opencv.hpp>
#include <chrono>

const int width = 2000;
const int height = 4000;
double array[width][height];

double fRand(double fMin, double fMax) {
    double f = (double) rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

int main() {
    for (auto &y: array) {
        for (auto &x: y) {
            x = fRand(0.0, 1.0);
        }
    }
    auto start = std::chrono::high_resolution_clock::now();

    cv::Mat A(width, height, CV_64F, array);

    auto finish = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count() << "ns\n";

    cv::imshow("array", A);
    cv::waitKey(0);

    return 0;
}

#include <cstdlib>
#include <iostream>
#include <string>

#include <opencv2/opencv.hpp>

int main(int argc, char* argv[]) {
    std::string input = "input.jpg";
    std::string output = "edges.jpg";

    if (argc > 1) {
        input = argv[1];
    }
    if (argc > 2) {
        output = argv[2];
    }

    cv::Mat img = cv::imread(input, cv::IMREAD_COLOR);
    if (img.empty()) {
        std::cout << "bad input image\n";
        return 1;
    }

    cv::Mat gray;
    cv::Mat blur_img;
    cv::Mat edges;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, blur_img, cv::Size(5, 5), 1.5);
    cv::Canny(blur_img, edges, 50, 150);

    cv::imwrite(output, edges);
    std::cout << "saved: " << output << "\n";

    const char* d = std::getenv("DISPLAY");
    if (d && d[0] != '\0') {
        cv::imshow("edges", edges);
        cv::waitKey(0);
    }

    return 0;
}

#include <cstdlib>
#include <iostream>
#include <string>

#include <opencv2/opencv.hpp>

int main(int argc, char* argv[]) {
    std::string input = "input.mp4";
    std::string output = "edges.mp4";
    if (argc > 1) {
        input = argv[1];
    }
    if (argc > 2) {
        output = argv[2];
    }

    cv::VideoCapture cap;
    if (input == "0") {
        cap.open(0);
    } else {
        cap.open(input);
    }

    if (!cap.isOpened()) {
        std::cout << "cant open video input\n";
        return 1;
    }

    int w = (int)cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int h = (int)cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    double fps = cap.get(cv::CAP_PROP_FPS);
    if (fps <= 0.0) {
        fps = 30.0;
    }

    cv::VideoWriter writer(
        output,
        cv::VideoWriter::fourcc('m', 'p', '4', 'v'),
        fps,
        cv::Size(w, h),
        true
    );

    if (!writer.isOpened()) {
        std::cout << "cant open output video\n";
        return 1;
    }

    const char* d = std::getenv("DISPLAY");
    bool show = (d && d[0] != '\0');

    cv::Mat frame;
    cv::Mat g;
    cv::Mat b;
    cv::Mat e;
    cv::Mat e_bgr;
    long long count = 0;

    while (cap.read(frame)) {
        cv::cvtColor(frame, g, cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(g, b, cv::Size(5, 5), 1.5);
        cv::Canny(b, e, 50, 150);
        cv::cvtColor(e, e_bgr, cv::COLOR_GRAY2BGR);

        writer.write(e_bgr);
        count++;

        if (show) {
            cv::imshow("edges_video", e);
            int key = cv::waitKey(1);
            if (key == 27 || key == 'q') {
                break;
            }
        }
    }

    std::cout << "done. frames: " << count << " output: " << output << "\n";
    return 0;
}

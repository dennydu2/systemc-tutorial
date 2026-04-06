#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/opencv.hpp>

int main(int argc, char* argv[]) {
    std::string input = "input.mp4";
    std::string output = "objects.mp4";
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
        // Try default backend first.
        cap.open(input, cv::CAP_ANY);
        if (!cap.isOpened()) {
            // Fallback for some Linux systems where ffmpeg works better.
            cap.open(input, cv::CAP_FFMPEG);
        }
    }

    if (!cap.isOpened()) {
        std::cout << "cant open video input\n";
        return 1;
    }

    int w = (int)cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int h = (int)cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    double fps = cap.get(cv::CAP_PROP_FPS);
    if (fps < 1.0 || fps > 240.0) {
        fps = 30.0;
    }
    int wait_ms = (int)(1000.0 / fps);
    if (wait_ms < 1) {
        wait_ms = 1;
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

    std::cout << "input fps: " << fps << "\n";

    const char* d = std::getenv("DISPLAY");
    bool show = (d && d[0] != '\0');

    // Standard OpenCV background subtraction for multi-object blobs.
    cv::Ptr<cv::BackgroundSubtractor> bg = cv::createBackgroundSubtractorMOG2(200, 25.0, true);

    cv::Mat frame;
    cv::Mat fgmask;
    cv::Mat clean;
    std::vector<std::vector<cv::Point> > contours;
    long long count = 0;

    while (cap.read(frame)) {
        bg->apply(frame, fgmask);

        cv::threshold(fgmask, fgmask, 200, 255, cv::THRESH_BINARY);
        cv::morphologyEx(fgmask, clean, cv::MORPH_OPEN, cv::Mat(), cv::Point(-1, -1), 1);
        cv::morphologyEx(clean, clean, cv::MORPH_DILATE, cv::Mat(), cv::Point(-1, -1), 2);

        contours.clear();
        cv::findContours(clean, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        int obj_id = 1;
        for (size_t i = 0; i < contours.size(); ++i) {
            double area = cv::contourArea(contours[i]);
            if (area < 600.0) {
                continue;
            }

            cv::Rect box = cv::boundingRect(contours[i]);
            cv::rectangle(frame, box, cv::Scalar(0, 255, 0), 2);
            cv::putText(frame,
                "obj_" + std::to_string(obj_id),
                cv::Point(box.x, box.y - 6),
                cv::FONT_HERSHEY_SIMPLEX,
                0.6,
                cv::Scalar(0, 255, 0),
                2);
            obj_id++;
        }

        writer.write(frame);
        count++;

        if (show) {
            cv::imshow("object_detection", frame);
            int key = cv::waitKey(wait_ms);
            if (key == 27 || key == 'q') {
                break;
            }
        }
    }

    std::cout << "done. frames: " << count << " output: " << output << "\n";
    return 0;
}

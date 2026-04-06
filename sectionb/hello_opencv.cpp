#include <cstdlib>
#include <iostream>

#include <opencv2/opencv.hpp>

int main(int argc, char* argv[]) {
	std::string path = "input.jpg";
	if (argc > 1) {
		path = argv[1];
	}

	cv::Mat img = cv::imread(path, cv::IMREAD_COLOR);
	if (img.empty()) {
		std::cout << "cant read image: " << path << "\n";
		return 1;
	}

	std::cout << "loaded " << img.cols << " x " << img.rows << "\n";

	const char* d = std::getenv("DISPLAY");
	if (d && d[0] != '\0') {
		cv::imshow("hello", img);
		cv::waitKey(0);
	} else {
		cv::imwrite("hello_copy.jpg", img);
		std::cout << "no display, saved hello_copy.jpg\n";
	}

	return 0;
}

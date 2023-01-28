#include <iostream>
#include <opencv2/opencv.hpp>

int main() {

	std::cout << "Hello world!";

	cv::Mat image = cv::imread("C:\\Users\\Krivoklatsky\\Pictures\\Einladung.png");
	cv::resize(image, image, image.size() / 2);
	cv::imshow("test window", image);

	cv::waitKey(0);

	return 0;
}
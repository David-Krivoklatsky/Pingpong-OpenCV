#include <iostream>
#include <opencv2/opencv.hpp>
#include <ctime>

int main() {

	/*
	std::cout << "Hello world!";
	
	cv::Mat image = cv::imread("C:\\Users\\Krivoklatsky\\Pictures\\Einladung.png");
	cv::resize(image, image, image.size() / 2);
	cv::imshow("test window", image);
	*/

	srand(time(0));

	// Vytvori okno
	cv::namedWindow("oknoo", cv::WindowFlags::WINDOW_NORMAL);

	// vygeneruje nahodne suradnice pre kruh
	int target_x = rand() % 500 + 50;
	int target_y = rand() % 500 + 50;

	// deklaruje skore
	int score = 0;

	// nastavi velkost 600x600 px a vyplni èiernou (zeros), to posledne neviem co robi
	cv::Mat frame = cv::Mat::zeros(600, 600, CV_8UC3);

	// vytvori kruh na suradnicach
	cv::circle(frame, cv::Point(target_x, target_y), 50, cv::Scalar(0, 0, 255), -1);

	imshow("oknoo", frame);
	/*
	while (true) {
		cv::Mat frame = cv::Mat::zeros(600, 600, CV_8UC3);

	}
	*/

	cv::waitKey(0);

	return 0;
}
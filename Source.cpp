#include <iostream>
#include <ctime>				//pre random èisla generovane na zaklade èasu

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>	//pre urèenie polohy myši

using namespace cv;
using namespace std;


int main() {
	//generuje random èisla pod¾a èasu
	srand(time(0));

	namedWindow("keygame", WINDOW_KEEPRATIO);

	int win_size = 600;
	int car_x = win_size/2, car_y = win_size/2;

	while (true) {
		Mat keygame_frame = Mat::zeros(win_size, win_size, CV_8UC3);
		rectangle(keygame_frame, Point(car_x - 10, car_y + 10), Point(car_x + 10, car_y - 10), Scalar(255, 0, 0), -1);
		imshow("keygame", keygame_frame);


		int pressed_key = waitKey(0);

		switch (pressed_key) {
		case 27: //escape key
			return 0; // exit program
			break;
		case 'w':
			car_y -= 10;
			break;
		case 's':
			car_y += 10;
			break;
		case 'a':
			car_x -= 10;
			break;
		case 'd':
			car_x += 10;
			break;
		default:
			break;
		}
	}

	return 0;
}
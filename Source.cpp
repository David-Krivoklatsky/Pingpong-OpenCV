#include <iostream>
#include <ctime>				//pre random èisla generovane na zaklade èasu

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>	//pre urèenie polohy myši

using namespace cv;
using namespace std;


int main() {
	//generuje random èisla pod¾a èasu
	srand(time(0));

	VideoCapture cap(0);

	if (!cap.isOpened())
		return 69;

	Mat frame;
	while (true) {
		cap >> frame;
		flip(frame, frame, 1);
		imshow("Camera", frame);
		if (waitKey(30) == 27)
			break;
	}

	namedWindow("keygame", WINDOW_KEEPRATIO);

	int win_size = 600,
		car_x = win_size / 2,
		car_y = win_size / 2,

		ball_size = 10,
		ball_x = rand() % (win_size - ball_size + 1),
		ball_y = rand() % (win_size - ball_size + 1),
		ball_x_speed = 1+rand()%11, //nahodna zmena x o cislo od 1 do 10
		ball_y_speed = 1+rand()%11;


	while (true) {
		Mat keygame_frame = Mat::zeros(win_size, win_size, CV_8UC3);
		rectangle(keygame_frame, Point(car_x - 10, car_y + 10), Point(car_x + 10, car_y - 10), Scalar(255, 0, 0), -1);


		if (ball_x <= ball_size || ball_x >= win_size - ball_size) ball_x_speed *= -1; // ak je lopta na kraji okna, odrazi sa
		if (ball_y <= ball_size || ball_y >= win_size - ball_size) ball_y_speed *= -1;

		ball_x += ball_x_speed; // moze to pricitat aj odcitat
		ball_y += ball_y_speed; // aj tu, kvoli tomu tam nie je ++

		circle(keygame_frame, Point(ball_x, ball_y), ball_size, Scalar(0, 255, 0), 1, 8, 0);

		putText(keygame_frame, "ball x speed: " + to_string(ball_x_speed), Point(5, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);
		putText(keygame_frame, "ball y speed: " + to_string(ball_y_speed), Point(5, 60), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);

		imshow("keygame", keygame_frame);


		int pressed_key = waitKey(1);

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
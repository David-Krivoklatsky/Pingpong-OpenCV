#include <iostream>
#include <ctime>				//pre random èisla generovane na zaklade èasu

#include <opencv2/opencv.hpp>
//#include <opencv2/dnn.hpp>
//#include <opencv2/highgui.hpp>	//pre urèenie polohy myši

using namespace cv;
using namespace std;

const int WIN_SIZE = 600;

struct Object
{
	int x;
	int y;
};

struct Ball
{
	int x;
	int y;
	int size;
	int x_speed;
	int y_speed;
};

Ball createBall()
{
	Ball ball;
	ball.size = 10;
	ball.x = rand() % (WIN_SIZE - 2*ball.size + 1) + ball.size;
	ball.y = rand() % (WIN_SIZE - 2*ball.size + 1) + ball.size;
	ball.x_speed = 1 + rand() % 11; //nahodna zmena x o cislo od 1 do 10
	ball.y_speed = 1 + rand() % 11;
	return ball;
}

int main() {
	//generuje random èisla pod¾a èasu
	srand(time(0));
/*
	VideoCapture cap(0);

	if (!cap.isOpened())
		return 1;

	//loaduje kaskadu na rozpoznanie tvare
	CascadeClassifier face_cascade;
	if (!face_cascade.load("C:\\opencv-4.7.0\\build\\etc\\haarcascades\\haarcascade_frontalface_default.xml")) {
		cerr << "Error loading face cascade\n";
		return 2;
	}

	vector<Rect> tvare;
	Mat frame;
	while (true) {
		cap >> frame;
		flip(frame, frame, 1);

		vector<Rect> tvare;
		face_cascade.detectMultiScale(frame, tvare, 2, 5); //tie dva parametre su znizenie kvality a presnost detekcie
		
		// Draw a bounding box around each detected face
		
		for (const cv::Rect& tvar : tvare) {
			rectangle(frame, tvar, Scalar(0, 255, 0), 2);
		}
		//for (size_t i = 0; i < tvare.size(); i++) {
		//	rectangle(frame, tvare[i], Scalar(0, 255, 0), 2);
		//}

		





		imshow("Camera", frame);
		if (waitKey(1) == 27 && !tvare.empty())
		{
			for (size_t i = 0; i < tvare.size(); i++)
			{
				auto image = frame(tvare[i]);
				imshow("tvar" + to_string(i), image);
			}

			//waitKey(0);
		}
	}
*/


	namedWindow("keygame", WINDOW_KEEPRATIO);

	Object sliderL{ 10, WIN_SIZE / 2 };
	Object sliderR{ WIN_SIZE - 10, WIN_SIZE / 2 };

	Ball ball = createBall();

	int loses = 0;

	while (true) {
		Mat keygame_frame = Mat::zeros(WIN_SIZE, WIN_SIZE, CV_8UC3);

		rectangle(keygame_frame, Point(sliderL.x - 10, sliderL.y - 100), Point(sliderL.x + 10, sliderL.y + 100), Scalar(255, 0, 0), -1);
		rectangle(keygame_frame, Point(sliderR.x - 10, sliderR.y - 100), Point(sliderR.x + 10, sliderR.y + 100), Scalar(255, 0, 0), -1);

		if (ball.x <= sliderL.x + 20 && (ball.y >= sliderL.y - 100 && ball.y <= sliderL.y + 100)) ball.x_speed *= -1;
		if (ball.x >= sliderR.x - 20 && (ball.y >= sliderR.y - 100 && ball.y <= sliderR.y + 100)) ball.x_speed *= -1;
		if (ball.y <= ball.size || ball.y >= WIN_SIZE - ball.size) ball.y_speed *= -1; // ak je lopta na vrchu/spodu okna, odrazi sa
		if (ball.x <= ball.size || ball.x >= WIN_SIZE - ball.size) {
			loses++;
			ball = createBall();
		}

		ball.x += ball.x_speed; // moze to pricitat aj odcitat
		ball.y += ball.y_speed; // aj tu, kvoli tomu tam nie je ++

		circle(keygame_frame, Point(ball.x, ball.y), ball.size, Scalar(0, 255, 0), 1, 8, 0);

		putText(keygame_frame, "ball x speed: " + to_string(ball.x_speed), Point(5, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);
		putText(keygame_frame, "ball y speed: " + to_string(ball.y_speed), Point(5, 60), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);
		putText(keygame_frame, "Loses: " + to_string(loses), Point(5, 90), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);

		imshow("keygame", keygame_frame);


		int pressed_key = waitKey(1);

		switch (pressed_key) {
		case 27: //escape key
			return 0; // exit program
			break;
		case 'w':
			sliderL.y -= 10;
			break;
		case 's':
			sliderL.y += 10;
			break;
		case '8':
			sliderR.y -= 10;
			break;
		case '5':
			sliderR.y += 10;
			break;
		default:
			break;
		}
	}


	return 0;
}
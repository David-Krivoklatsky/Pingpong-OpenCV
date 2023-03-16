#include <iostream>
#include <ctime>				//pre random Ëisla generovane na zaklade Ëasu

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

//const int WIN_SIZE = 600;
const int SPEED = 20;

struct Object
{
	int x;
	int y;
};

struct Slider
{
	int x;
	int y;
	int width = 20;
	int height = 200;
	Scalar color{ 255,0,0 };
};

struct Ball
{
	double x;
	double y;
	int size;
	double x_speed;
	double y_speed;
};

double degToRad(double deg)
{
	return deg / 180 * CV_PI;
}

void drawRectangle(Mat& frame, const Slider& slider)
{
	rectangle(frame, Point(slider.x - slider.width / 2, slider.y - slider.height / 2), Point(slider.x + slider.width / 2, slider.y + slider.height / 2), slider.color, -1);
}

Point2f getCenter(const vector<Point2f>& points)
{
	Point2f sum;
	for (auto& p: points)
	{
		sum += p;
	}
	return sum / (int)points.size();
}

Ball createBall(Point2f position)
{
	Ball ball;
	ball.size = 10;
	ball.x = position.x;
	ball.y = position.y;

	double angle = (rand() % 900 - 450) /10;

	ball.x_speed = cos(degToRad(angle)) * SPEED;
	ball.y_speed = sin(degToRad(angle)) * SPEED;
	return ball;
}

int main() {
	namedWindow("GAME", WINDOW_KEEPRATIO);

	Size res{ 1280, 720 };

	Ball ball = createBall(Point2f(res.width, res.height));

	Slider sliderL{ 10, res.height / 2 };
	Slider sliderR{ res.width - 10, res.height / 2 };

	VideoCapture cam(0);

	if (!cam.isOpened())
		return 1;
	
	cam.set(CAP_PROP_FRAME_WIDTH, 1280);
	cam.set(CAP_PROP_FRAME_HEIGHT, 720);

	
	int loses = 0;

	while (true) {

		Mat frame;
		cam >> frame;

		vector<int> markerIds;
		vector<vector<Point2f>> markerCorners, rejectedCandidates;
		aruco::DetectorParameters detectorParams = aruco::DetectorParameters();
		aruco::Dictionary dictionary = aruco::getPredefinedDictionary(aruco::DICT_6X6_100);
		aruco::ArucoDetector detector(dictionary, detectorParams);
		detector.detectMarkers(frame, markerCorners, markerIds, rejectedCandidates);

		aruco::drawDetectedMarkers(frame, markerCorners, markerIds);

		flip(frame, frame, 1);

		drawRectangle(frame, sliderL);
		drawRectangle(frame, sliderR);


		map<int /*id*/, vector<Point2f> /*corners*/> values;
		for (size_t i = 0; i < markerCorners.size(); i++)
		{
			values[markerIds[i]] = markerCorners[i];
		}

		if (values.find(39) != values.end())
		{
			sliderL.y = values[39][0].y;
		}

		if (values.find(77) != values.end())
		{
			sliderR.y = values[77][0].y;
		}


		if (ball.x <= (sliderL.x + 20) && (ball.y >= sliderL.y - 100 && ball.y <= sliderL.y + 100) ||
			ball.x >= (sliderR.x - 20) && (ball.y >= sliderR.y - 100 && ball.y <= sliderR.y + 100)) {
			ball.x_speed *= -1;
		}

		// ak je lopta na vrchu/spodu okna, odrazi sa
		if (ball.y <= ball.size || ball.y >= frame.size().height - ball.size) {
			ball.y_speed *= -1; 
		}

		if (ball.x <= ball.size || ball.x >= frame.size().width - ball.size) {
			loses++;
			ball = createBall({ 1280 / 2, 720 / 2 });
		}

		ball.x += ball.x_speed; // moze to pricitat aj odcitat
		ball.y += ball.y_speed; // aj tu, kvoli tomu tam nie je ++

		circle(frame, Point(ball.x, ball.y), ball.size, Scalar(0, 255, 0), -1, 8, 0);

		putText(frame, "Loses: " + to_string(loses), Point(5, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);
		
		imshow("GAME", frame);

		//ked to tu nie je tak to nefunguje
		waitKey(1);

		/*
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
		*/
	}
	return 0;
}

	/*
	»o pridaù:

	- SK”RE
	- Rozdielne odr·ûanie v z·vislosti od miesta odrazu
	- Moûnosù vybraù si medzi ovl·danÌm pomocou ARUCO alebo öÌpkami
	- HudbiËku

	*/
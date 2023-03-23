#include <iostream>
#include <ctime>				//pre random Ëisla generovane na zaklade Ëasu
#include <deque>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

//const int WIN_SIZE = 600;
const int SPEED = 10;
//const int FRAME_REUSE_COUNT = 2;
const int FPS = 120;

deque<Mat> frameQueue;
mutex frameMutex;
bool stopCapThread = false;

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
	int radius;
	double x_speed;
	double y_speed;
};

double degToRad(double deg)
{
	return deg / 180 * CV_PI;
}

void drawSlider(Mat& frame, const Slider& slider)
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
	ball.radius = 10;
	ball.x = position.x;
	ball.y = position.y;

	double angle = (rand() % 900 - 450) /10;

	ball.x_speed = cos(degToRad(angle)) * SPEED;
	ball.y_speed = sin(degToRad(angle)) * SPEED;
	return ball;
}

void camCaptureThread(VideoCapture& cam)
{
	while (!stopCapThread)
	{
		Mat frame;
		cam >> frame;

		lock_guard<mutex> lk(frameMutex);
		frameQueue.push_back(frame);

		const int MAX_QUEUE_SIZE = 5;
		if (frameQueue.size() > MAX_QUEUE_SIZE)
			frameQueue.pop_front();
	}
}

Mat getNewFrame()
{
	lock_guard<mutex> lk(frameMutex);

	Mat frame;
	if (!frameQueue.empty())
	{
		frame = frameQueue.front();
		frameQueue.pop_front();
	}

	return frame;
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

	
	int scoreL = 0, scoreR = 0;

	

	thread capThread(camCaptureThread, std::ref(cam));

	
	while (true) {

		Mat frame;
		Mat newFrame = getNewFrame();
		if (!newFrame.empty())
			frame = newFrame;

		if (frame.empty()) {
			this_thread::sleep_for(100ms);
			continue;
		}

		vector<int> markerIds;
		vector<vector<Point2f>> markerCorners, rejectedCandidates;
		aruco::DetectorParameters detectorParams = aruco::DetectorParameters();
		aruco::Dictionary dictionary = aruco::getPredefinedDictionary(aruco::DICT_6X6_100);
		aruco::ArucoDetector detector(dictionary, detectorParams);
		detector.detectMarkers(frame, markerCorners, markerIds, rejectedCandidates);

		aruco::drawDetectedMarkers(frame, markerCorners, markerIds);

		flip(frame, frame, 1);

		drawSlider(frame, sliderL);
		drawSlider(frame, sliderR);


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

		// ak je lopta na vrchu/spodu okna, odrazi sa
		if (ball.y <= ball.radius || ball.y >= frame.size().height - ball.radius) {
			ball.y_speed *= -1; 
		}

		//zmenit tak aby to pocitalo s kruhom a nie kde je x a y, chapes
		if (ball.x <= (sliderL.x + sliderL.width / 2 + ball.radius) && (ball.y >= sliderL.y - sliderL.height / 2 && ball.y <= sliderL.y + sliderL.height / 2) ||
			ball.x >= (sliderR.x - sliderR.width / 2 - ball.radius) && (ball.y >= sliderR.y - sliderR.height / 2 && ball.y <= sliderR.y + sliderL.height / 2)) {
			ball.x_speed *= -1;
		}



		if (ball.x <= ball.radius) {
			scoreR++;
			ball = createBall({ 1280 / 2, 720 / 2 });
		}
		if (ball.x >= frame.size().width - ball.radius) {
			scoreL++;
			ball = createBall({ 1280 / 2, 720 / 2 });
		}

		ball.x += ball.x_speed; // moze to pricitat aj odcitat
		ball.y += ball.y_speed; // aj tu, kvoli tomu tam nie je ++

		circle(frame, Point(ball.x, ball.y), ball.radius, Scalar(0, 255, 0), -1, 8, 0);

		//skÛre
		putText(frame, to_string(scoreL) + " " + to_string(scoreR), Point(1280 / 2, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);
		
		imshow("GAME", frame);

		int key = waitKey(1);
		if (key == 27) {
			break;
		}

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

		//this_thread::sleep_for(chrono::milliseconds(1000 / FPS));
	}

	stopCapThread = true;
	capThread.join();

	return 0;
}

	/*
	»o pridaù:

	- zmeniù odraûanie tak aby sa to odrazilo aj na rohu
	- SK”RE
	- Vybraù si kÛd ARUCO kodu
	- Rozdielne odr·ûanie v z·vislosti od miesta odrazu
	- Moûnosù vybraù si medzi ovl·danÌm pomocou ARUCO alebo öÌpkami
	- HudbiËku

	*/
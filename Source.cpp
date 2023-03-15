#include <iostream>
#include <ctime>				//pre random èisla generovane na zaklade èasu

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

cv::Point2f getCenter(const std::vector<cv::Point2f>& points)
{
	cv::Point2f sum;
	for (auto& p: points)
	{
		sum += p;
	}
	return sum / (int)points.size();
}

Ball createBall(cv::Point2f position)
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

	cv::Size res{ 1280, 720 };

	Ball ball = createBall(cv::Point2f(res.width, res.height));

	Slider sliderL{ 10, res.height / 2 };
	Slider sliderR{ res.width - 10, res.height / 2 };

	int loses = 0;

	VideoCapture cam(0);
	cam.set(CAP_PROP_FRAME_WIDTH, 1280);
	cam.set(CAP_PROP_FRAME_HEIGHT, 720);

	while (true) {

		cv::Mat frame;
		cam >> frame;

		std::vector<int> markerIds;
		std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;
		cv::aruco::DetectorParameters detectorParams = cv::aruco::DetectorParameters();
		cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_100);
		cv::aruco::ArucoDetector detector(dictionary, detectorParams);
		detector.detectMarkers(frame, markerCorners, markerIds, rejectedCandidates);

		cv::Mat outputImage = frame.clone();
		cv::aruco::drawDetectedMarkers(outputImage, markerCorners, markerIds);
		cv::flip(outputImage, outputImage, 1);
		imshow("cam", outputImage);

		//Mat keygame_frame = Mat::zeros(WIN_SIZE, WIN_SIZE, CV_8UC3);
		Mat keygame_frame = outputImage * 0.7;

		drawRectangle(keygame_frame, sliderL);
		drawRectangle(keygame_frame, sliderR);


		std::map<int /*id*/, std::vector<cv::Point2f> /*corners*/> values;
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

		if (ball.y <= ball.size || ball.y >= frame.size().height - ball.size)
			ball.y_speed *= -1; // ak je lopta na vrchu/spodu okna, odrazi sa

		if (ball.x <= ball.size || ball.x >= frame.size().width - ball.size) {
			loses++;
			ball = createBall({ 1280 / 2, 720 / 2 });
		}

		ball.x += ball.x_speed; // moze to pricitat aj odcitat
		ball.y += ball.y_speed; // aj tu, kvoli tomu tam nie je ++

		circle(keygame_frame, Point(ball.x, ball.y), ball.size, Scalar(0, 255, 0), -1, 8, 0);

		putText(keygame_frame, "ball x speed: " + to_string(ball.x_speed), Point(5, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);
		putText(keygame_frame, "ball y speed: " + to_string(ball.y_speed), Point(5, 60), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);
		putText(keygame_frame, "Loses: " + to_string(loses), Point(5, 90), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);

		//cv::flip(keygame_frame, keygame_frame, 1);
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
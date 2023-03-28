#include "Game.h"
#include <ctime>				//pre random èisla generovane na zaklade èasu

using namespace std;
using namespace cv;

double degToRad(double deg)
{
	return deg / 180 * CV_PI;
}

Point2f getCenter(const vector<Point2f>& points)
{
	Point2f sum;
	for (auto& p : points)
	{
		sum += p;
	}
	return sum / (int)points.size();
}

Game::Game(int speed, int fps, Size resolution)
	: speed(speed)
	, fps(fps)
	, resolution(resolution)
	, ball(createBall(Point2f(resolution.width, resolution.height) / 2))
	, sliderL{ 10, resolution.height / 2 }
	, sliderR{ resolution.width - 10, resolution.height / 2 }
	, renderFrame(Mat::zeros(resolution, CV_8UC3))
{
	// Spustime thread kde bude bezat hra
	renderThread = std::thread(&Game::gameLoop, this);
}

Game::~Game()
{
	// Zastavime thread kde bezi hra
	stopRenderThread = true;
	if (renderThread.joinable())
		renderThread.join();
}

void Game::setFrame(const Mat& frame)
{
	lock_guard<mutex> lk(frameMutex);
	renderFrame = frame;
}

void Game::gameLoop()
{
	using clock = chrono::steady_clock;

	namedWindow("GAME", WINDOW_KEEPRATIO);

	Mat frame;
	int scoreL = 0;
	int scoreR = 0;

	while (!stopRenderThread)
	{
		auto t_start = clock::now();

		// Skopirujeme posledny frame (zamkneme, aby nam nikto pocas kopirovania fotku neupravil)
		{
			lock_guard<mutex> lk(frameMutex);
			frame = renderFrame.clone();
		}

		//aruco::drawDetectedMarkers(frame, markerCorners, markerIds);

		moveBall(frame);

		if (ball.x <= ball.radius) {
			scoreR++;
			ball = createBall(Point2f(resolution.width, resolution.height) / 2);
		}
		if (ball.x >= frame.size().width - ball.radius) {
			scoreL++;
			ball = createBall(Point2f(resolution.width, resolution.height) / 2);
		}

		// Render
		{
			lock_guard<mutex> lk(sliderMutex);
			drawSlider(frame, sliderL);
			drawSlider(frame, sliderR);
		}
		drawBall(frame, ball);

		//skóre
		putText(frame, to_string(scoreL) + " " + to_string(scoreR), Point(1280 / 2, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);

		imshow("GAME", frame);

		int key = waitKey(1);
		if (key == 27) {
			stopRenderThread = true;
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

		// Pockame tolko aby sme zachovali FPS
		auto elapsed = clock::now() - t_start;
		chrono::milliseconds period(1000 / fps);
		this_thread::sleep_for(period - elapsed);
	}

	destroyWindow("GAME");
}

void Game::moveBall(const Mat& frame)
{
	// Zamkneme aby nam nikto nezmodifikoval polohu sliderov
	lock_guard<mutex> lk(sliderMutex);

	// ak je lopta na vrchu/spodu okna, odrazi sa
	if (ball.y <= ball.radius || ball.y >= frame.size().height - ball.radius) {
		ball.y_speed *= -1;
	}

	//zmenit tak aby to pocitalo s kruhom a nie kde je x a y, chapes
	if (ball.x <= (sliderL.x + sliderL.width / 2 + ball.radius) && (ball.y >= sliderL.y - sliderL.height / 2 && ball.y <= sliderL.y + sliderL.height / 2) ||
		ball.x >= (sliderR.x - sliderR.width / 2 - ball.radius) && (ball.y >= sliderR.y - sliderR.height / 2 && ball.y <= sliderR.y + sliderL.height / 2)) {
		ball.x_speed *= -1;
	}

	ball.x += ball.x_speed * speed; // moze to pricitat aj odcitat
	ball.y += ball.y_speed * speed; // aj tu, kvoli tomu tam nie je ++
}

void Game::drawBall(Mat& frame, const Ball& ball)
{
	circle(frame, Point(ball.x, ball.y), ball.radius, ball.color, -1, 8, 0);
}

void Game::drawSlider(Mat& frame, const Slider& slider)
{
	rectangle(frame, Point(slider.x - slider.width / 2, slider.y - slider.height / 2), Point(slider.x + slider.width / 2, slider.y + slider.height / 2), slider.color, -1);
}

Ball Game::createBall(cv::Point2f position)
{
	Ball ball;
	ball.y = position.y;

	double angle = (rand() % 1800 - 450) / 10.0;
	if (angle > 45)
	{
		angle += 90;
		ball.x = position.x * 1.5;
	}
	else
	{
		ball.x = position.x * 0.5;
	}

	ball.x_speed = cos(degToRad(angle));
	ball.y_speed = sin(degToRad(angle));
	return ball;
}

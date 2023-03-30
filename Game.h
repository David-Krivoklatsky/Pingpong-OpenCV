#pragma once
#include <opencv2/opencv.hpp>

struct Slider
{
	int x;
	int y;
	int width = 20;
	int height = 200;
	cv::Scalar color{ 255,0,0 };
};

struct Ball
{
	double x;
	double y;
	int radius = 10;
	double x_speed;
	double y_speed;
	cv::Scalar color{ 0, 255, 0 };
};

class Game
{
public:
	Game(int speed = 5, int fps = 120, cv::Size resolution = cv::Size{ 1280, 720 });
	~Game();

	void setFrame(const cv::Mat& frame);
	void setSliderL(int position);
	void setSliderR(int position);
	bool isRunning() const { return !stopRenderThread; }

private:
	void gameLoop();
	void moveBall(const cv::Mat& frame);
	void drawBall(cv::Mat& frame, const Ball& ball);
	void drawSlider(cv::Mat& frame, const Slider& slider);
	Ball createBall(cv::Point2f position);

private:
	int speed;
	int fps;
	cv::Size resolution;

	Ball ball;
	Slider sliderL;
	Slider sliderR;
	std::mutex sliderMutex;

	cv::Mat renderFrame = cv::Mat::zeros(resolution, CV_8UC3);
	std::mutex frameMutex;
	bool stopRenderThread = false;
	std::thread renderThread;
};

#include <iostream>
#include "Game.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

const int ARUCO_L_ID = 39;
const int ARUCO_R_ID = 77;

map<int, vector<Point2f>> detectArucos(const Mat& frame)
{
	vector<int> markerIds;
	vector<vector<Point2f>> markerCorners;
	aruco::DetectorParameters detectorParams = aruco::DetectorParameters();
	aruco::Dictionary dictionary = aruco::getPredefinedDictionary(aruco::DICT_6X6_100);
	aruco::ArucoDetector detector(dictionary, detectorParams);
	detector.detectMarkers(frame, markerCorners, markerIds);

	map<int /*id*/, vector<Point2f> /*corners*/> values;
	for (size_t i = 0; i < markerCorners.size(); i++)
	{
		values[markerIds[i]] = markerCorners[i];
	}

	return values;
}

int main()
{
	VideoCapture cam(1);

	if (!cam.isOpened())
		return 1;
	
	cam.set(CAP_PROP_FRAME_WIDTH, 1280);
	cam.set(CAP_PROP_FRAME_HEIGHT, 720);

	// Spustime hru
	Game game(6, 120);

	while (game.isRunning())
	{
		Mat frame;
		cam >> frame;

		// Najdeme aruco znacky v originalnom neotocenom obraze
		auto arucos = detectArucos(frame);
		if (!arucos.empty())
		{
			// Ak sa v obraze nasla znacka aruco pre lavy slider, upravime jeho polohu
			if (arucos.find(ARUCO_L_ID) != arucos.end())
			{
				game.setSliderL(arucos[ARUCO_L_ID][0].y);
			}

			// Ak sa v obraze nasla znacka aruco pre pravy slider, upravime jeho polohu
			if (arucos.find(ARUCO_R_ID) != arucos.end())
			{
				game.setSliderR(arucos[ARUCO_R_ID][0].y);
			}
		}

		// Flipneme obraz aby bol zrkadlovo otoceny 
		flip(frame, frame, 1);

		game.setFrame(frame);
	}

	return 0;
}

	/*
	»o pridaù:

	- zmeniù odraûanie tak aby sa to odrazilo aj na rohu
	- SK”RE
	- Vybraù si kÛd ARUCO kodu
	- Rozdielne odr·ûanie v z·vislosti od miesta odrazu
	- moûnosù vybraù si kameru
	- Moûnosù vybraù si medzi ovl·danÌm pomocou ARUCO alebo öÌpkami
	- HudbiËku

	*/
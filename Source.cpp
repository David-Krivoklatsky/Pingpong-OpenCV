#include <iostream>
#include <ctime>				//pre random �isla generovane na zaklade �asu

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>	//pre ur�enie polohy my�i

using namespace cv;
using namespace std;



/*
int score, target_x, target_y; // glob�lne premenn�

// funkcia, ktora checkne �i je stla�ene lave tla�idlo my�i, �i bolo to kliknutie v radiuse a potom pripo�ita skore++ a na koniec vytvori novy target
void onMouse(int event, int x, int y, int flag, void* userdata) {
	if (event == EVENT_LBUTTONDOWN) {

		// vypocita vzdialenost, je to na 2, cize je jedno ci je cislo zaporne
		double distance = sqrt(pow(x - target_x, 2) + pow(y - target_y, 2));
		if (distance < 50) {
			score++;

			//generujem nove suradnice
			target_x = rand() % 500 + 50; // 50 a� 550 px
			target_y = rand() % 500 + 50; // 50 a� 550 px
		}
	}
}
*/



int main() {
	//generuje random �isla pod�a �asu
	srand(time(0));

/*

	//PRVA HRA

	// Vytvori okno
	namedWindow("clickgame", WINDOW_KEEPRATIO);

	// vygeneruje nahodne suradnice pre kruh
	target_x = rand() % 500 + 50; // 50 a� 550 px
	target_y = rand() % 500 + 50; // 50 a� 550 px


	while (true) {
		// nastavi velkost 600x600 px a vyplni �iernou (zeros), to posledne neviem co robi
		Mat frame = Mat::zeros(600, 600, CV_8UC3);

		// uka�e skore (dalo by sa zv��i� ke� je 3 ciferne �islo)
		putText(frame, "Score: " + to_string(score), Point(440, 40),
		FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);

		// vytvori kruh na suradnicach, farba - cervena
		circle(frame, Point(target_x, target_y), 50, Scalar(0, 0, 255), -1);

		//ukaze okno uz vyplnene ciernou farbou
		imshow("clickgame", frame);

		// funkcia, ktora zavola funkciu onMouse pre konkretne okno
		setMouseCallback("clickgame", onMouse);
	
		//toto tu musi by� inak to nefunguje, ke� je tam waitKey(0), tak to �aka kym stla�im nulu
		waitKey(1);
	}

*/

	//DRUHA HRA

	namedWindow("keygame", WINDOW_KEEPRATIO);

	int win_size = 600;
	int car_x = win_size/2, car_y = win_size/2;

	int pressed_key = waitKey(0);

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
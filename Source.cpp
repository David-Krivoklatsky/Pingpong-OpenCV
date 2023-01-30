#include <iostream>
#include <ctime>				//pre random �isla generovane na zaklade �asu

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>	//pre ur�enie polohy my�i

using namespace cv;
using namespace std;

int score, target_x, target_y; // glob�lne premenn�

// funkcia, ktora checkne �i je stla�ene lave tla�idlo my�i, �i bolo to kliknutie v radiuse a potom pripo�ita skore++ a an koniec vytvori novy target
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

int main() {
	srand(time(0));

	// Vytvori okno
	namedWindow("oknoo", WindowFlags::WINDOW_NORMAL);

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
		imshow("oknoo", frame);

		// funkcia, ktora zavola funkciu onMouse pre konkretne okno
		setMouseCallback("oknoo", onMouse);
	
		//toto tu musi by� inak to nefunguje, ke� je tam waitKey(0), tak to �aka kym stla�im nulu
		waitKey(1);
	}

	return 0;
}
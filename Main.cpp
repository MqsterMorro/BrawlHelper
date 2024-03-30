
#include <opencv2/opencv.hpp>
#include <Windows.h>
#include <thread>
#include <map>

#include "Object.h"
#include "WindowUtils.h"
#include "Bot.h"

cv::Point realPlayer = cv::Point(0, 0);
std::vector<Object> objects;
std::thread move;

cv::Point nearestEnemy;

LPCWSTR windowTitle = L"BlueStacks App Player";
HWND hWND = FindWindow(NULL, windowTitle);
HWND hwndChild = GetWindow(hWND, GW_CHILD);


int tileWidth = 24;
int tileHeight = 17;
int choice;
void askInput() {
	std::cout << "what action to take: \n";
	std::cout << "1. go to closest enemy \n";
	std::cout << "2. shoot closest enemy \n";
	double speed = 2.57;
	std::cin >> choice;
	if (choice == 1) {
		double tilePixelWidth = getWindowSize(hwndChild)[0] / tileWidth;
		double tilePixelHeight = getWindowSize(hwndChild)[1] / tileHeight;
		std::cout << "how many tiles between u and nearest enemy (x): " << std::to_string(getDistanceX(getSpecificObject(Player, objects).position, getNearestEnemy(getSpecificObject(Player, objects).position, objects)) / tilePixelWidth) << "\n";
		double time = (getDistanceX(getSpecificObject(Player, objects).position, getNearestEnemy(getSpecificObject(Player, objects).position, objects)) / tilePixelWidth) / speed;
		double time2 = (getDistanceY(getMid(getSpecificObject(Player, objects).rect), getNearestEnemy(getMid(getSpecificObject(Player, objects).rect), objects)) / tilePixelWidth) / speed;
		std::cout << "old time: " << std::to_string(time) << "\n";
		std::cout << "new time: " << std::to_string(time2) << "\n";

		char key = getAppropriateKeyX(getSpecificObject(Player, objects).position, getNearestEnemy(getSpecificObject(Player, objects).position, objects));
		char key2 = getAppropriateKeyY(getSpecificObject(Player, objects).position, getNearestEnemy(getSpecificObject(Player, objects).position, objects));

		keepDown(hwndChild, key, time);
		keepDown(hwndChild, key2, time2);
		PostMessage(hwndChild, WM_KEYUP, key, 0);
		PostMessage(hwndChild, WM_KEYUP, key2, 0);
		askInput();
		
	}

	else if (choice == 2) {


		shootCoordinate(hwndChild, getNearestEnemy(getSpecificObject(Player, objects).position, objects));
		
		askInput();
	}

	else {
		std::cout << "wrong."; 
		askInput();
	}
	askInput();
}

void movePlayer() {

	while (getDistance(getSpecificObject(Player, objects).position, getNearestEnemy(getSpecificObject(Player, objects).position, objects)) > 3) {

		std::cout << "x: " << std::to_string(getNearestEnemy(getSpecificObject(Player, objects).position, objects).x) << std::endl;
		std::cout << "y: " << std::to_string(getNearestEnemy(getSpecificObject(Player, objects).position, objects).y) << std::endl;
		std::cout << "x2: " << getAppropriateKeyX(getSpecificObject(Player, objects).position, getNearestEnemy(getSpecificObject(Player, objects).position, objects));
		std::cout << "y2: " << getAppropriateKeyY(getSpecificObject(Player, objects).position, getNearestEnemy(getSpecificObject(Player, objects).position, objects));
		clickKey(hwndChild, getAppropriateKeyX(getSpecificObject(Player, objects).position, getNearestEnemy(getSpecificObject(Player, objects).position, objects)), "down");
		clickKey(hwndChild, getAppropriateKeyY(getSpecificObject(Player, objects).position, getNearestEnemy(getSpecificObject(Player, objects).position, objects)), "down");
	}
	clickKey(hwndChild, getAppropriateKeyX(getSpecificObject(Player, objects).position, getNearestEnemy(getSpecificObject(Player, objects).position, objects)), "up");
	clickKey(hwndChild, getAppropriateKeyY(getSpecificObject(Player, objects).position, getNearestEnemy(getSpecificObject(Player, objects).position, objects)), "up");
}

cv::Scalar playerLow = cv::Scalar(51, 136, 155);
cv::Scalar playerHigh = cv::Scalar(59, 196, 215);
cv::Scalar enemyLow = cv::Scalar(1, 147, 201);
cv::Scalar enemyHigh = cv::Scalar(2, 178, 255);
cv::Scalar bushLow = cv::Scalar(56, 175, 128);
cv::Scalar bushHigh = cv::Scalar(68, 133, 129);
cv::Scalar allyLow = cv::Scalar(94, 133, 129);
cv::Scalar allyHigh = cv::Scalar(107, 158, 251);

void detect() {
	int key = 0;
	while (key != 27) {
		cv::Mat target = getMat(hWND);
		cv::Mat background;
		target.copyTo(background);

		objects.clear();
		cv::cvtColor(target, target, cv::COLOR_BGR2HSV);
		cv::Mat maskk;
		cv::inRange(target, enemyLow, enemyHigh, maskk);
		getObjects(target, playerLow, playerHigh, Player, objects);
		getObjects(target, enemyLow, enemyHigh, Enemy, objects);
		getObjects(target, allyLow, allyHigh, Ally, objects);
		drawObjects(background, objects);

		cv::imshow("contours", background);


		key = cv::waitKey(250);
	}
}




int main()
{
	cv::Point pp;
	std::thread detector(detect);
	std::cout << pp;
	Sleep(1000);
	std::cout << "ok moving";
	//std::thread move(movePlayer);
	askInput();
	detector.join();
	//move.join();





}

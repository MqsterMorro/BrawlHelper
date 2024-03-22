

#include "DarkHelp.hpp"
#include <Windows.h>
#include <thread>
#include <map>

#include "Object.h"
#include "WindowsUtils.h"

cv::Point realPlayer = cv::Point(0, 0);
std::vector<Object> objects;
std::thread move;


cv::Point nearestEnemy;


LPCWSTR windowTitle = L"BlueStacks App Player";
HWND hWND = FindWindow(NULL, windowTitle);
HWND hwndChild = GetWindow(hWND, GW_CHILD);




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
	Sleep(5000);
	std::cout << "ok moving";
	std::thread move(movePlayer);
	detector.join();
	move.join();
	
	
	
	
	
}


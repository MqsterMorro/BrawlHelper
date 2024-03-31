
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

BotState State = SEARCHING;
int tileWidth = 24;
int tileHeight = 17;
int choice;
double tilePixelWidth;
double tilePixelHeight;
double myTime;
double time2;
double time3;
double tileDistanceX;
double tileDistanceY;
double tileDistanceDiagonal;
double key;
double key2;
double speed = 3;
Object lastPlayer;
double iteration;

void doBotState(BotState botState) {
	
	/*if (getSpecificObject(Player, objects).empty() || getSpecificObject(Enemy, objects).empty()) {
		objects.emplace_back(lastPlayer);
		std::cout << "i was executted! \n";
		
	}
	else {
		std::cout << "iteration is: " << std::to_string(iteration) << "\n";
		std::cout << "tileDistanceDiagonal is: " << std::to_string(tileDistanceDiagonal) << "\n";
	}*/
		switch (botState) {
		case SEARCHING:
			Sleep(500);

					  tilePixelWidth = getWindowSize(hwndChild)[0] / tileWidth;
					  tilePixelHeight = getWindowSize(hwndChild)[1] / tileHeight;
	
					  if (!getSpecificObject(Player, objects).empty() && !getSpecificObject(Enemy, objects).empty()) {
						  iteration = 0;
						  if (!getSpecificObject(Player, objects).empty() && !getSpecificObject(Enemy, objects).empty()) {
							  std::cout << "how many tiles between u and nearest enemy (x): " << std::to_string(getDistanceX(getSpecificObject(Player, objects)[0].position, getNearestEnemy(getSpecificObject(Player, objects)[0].position, objects)) / tilePixelWidth) << "\n";
							  myTime = (getDistanceX(getSpecificObject(Player, objects)[0].position, getNearestEnemy(getSpecificObject(Player, objects)[0].position, objects)) / tilePixelWidth) / speed;
							  time2 = (getDistanceY(getMid(getSpecificObject(Player, objects)[0].rect), getNearestEnemy(getMid(getSpecificObject(Player, objects)[0].rect), objects)) / tilePixelWidth) / speed;
						  }
						  else {
							  std::cout << "Error getting Distances! \n";
							  std::cout << "Searching again! \n";
							  doBotState(SEARCHING);
							  break;
						  }
						  tileDistanceX = (getDistanceX(getSpecificObject(Player, objects)[0].position, getNearestEnemy(getSpecificObject(Player, objects)[0].position, objects)) / tilePixelWidth);
						  tileDistanceY = (getDistanceY(getMid(getSpecificObject(Player, objects)[0].rect), getNearestEnemy(getMid(getSpecificObject(Player, objects)[0].rect), objects)) / tilePixelWidth);
						  tileDistanceDiagonal = sqrt(std::pow(tileDistanceX, 2) + std::pow(tileDistanceY, 2));
						  std::cout << "x: " << std::to_string(tileDistanceX) << "\n";
						  std::cout << "y: " << std::to_string(tileDistanceY) << "\n";
						  time3 = tileDistanceDiagonal / speed;
						  if (!getSpecificObject(Player, objects).empty() && !getSpecificObject(Enemy, objects).empty()) {
						  key = getAppropriateKeyX(getSpecificObject(Player, objects)[0].position, getNearestEnemy(getSpecificObject(Player, objects)[0].position, objects));
						  key2 = getAppropriateKeyY(getSpecificObject(Player, objects)[0].position, getNearestEnemy(getSpecificObject(Player, objects)[0].position, objects));
						  }
						  else {
							  std::cout << "Error getting movement keys! \n";
							  std::cout << "Searching again! \n";
							  doBotState(SEARCHING);
							  break;
						  }
						  State = WALKING;
						  doBotState(WALKING);
						  std::cout << "bro idkfk: \n";
						  lastPlayer = getSpecificObject(Player, objects)[0];
						  break;
					  }
					  else if (getSpecificObject(Player, objects).empty()) {
						  std::cout << "Empty Player list! \n";
						  std::cout << "Searching again! \n";
						  iteration += 1;
						  std::cout << "Iteration: " << iteration << "\n";
						  if (iteration > 4) {
								std::cout << "Initiating LOST_PLAYER! \n";
								doBotState(LOST_PLAYER);
								break;
						  }
						  doBotState(SEARCHING);
						  break;
					  }

		case WALKING:
			if (abs(tileDistanceX - tileDistanceY) < 0.7) {
				std::cout << "Diagonal! \n";
				std::thread moveX(keepDown, hwndChild, key, time3);
				std::thread moveY(keepDown, hwndChild, key2, time3);
				moveY.join();
				moveX.join();
			}
			else {
				std::cout << "not Diagonal! \n";
				keepDown(hwndChild, key, myTime);
				keepDown(hwndChild, key2, time2);
			}
			State = ATTACKING;
			doBotState(ATTACKING);
			break;

		case ATTACKING:
			if (getSpecificObject(Player, objects).empty()) {
				std::cout << "eyah idkd im here 11 \n";
				objects.emplace_back(lastPlayer);
			}
			if (getSpecificObject(Enemy, objects).empty()) {
				std::cout << "No enemies were found! Initiating SEARCH \n";
				doBotState(SEARCHING);
				break;
			}
			if (sqrt(std::pow((getDistanceX(getSpecificObject(Player, objects)[0].position, getNearestEnemy(getSpecificObject(Player, objects)[0].position, objects)) / tilePixelWidth), 2)
				+ std::pow((getDistanceY(getMid(getSpecificObject(Player, objects)[0].rect), getNearestEnemy(getMid(getSpecificObject(Player, objects)[0].rect), objects)) / tilePixelWidth), 2)) < 4) {
				autoAim(hwndChild, 0);
				doBotState(ATTACKING);
				break;
			}
			else {

			
			State = SEARCHING;
			doBotState(SEARCHING);
			break;
			}
			
		case LOST_PLAYER:
			std::cout << "Player is lost! \n";
			std::cout << "Trying to find...! \n";
			keepDown(hwndChild, 'W', 0.5);
			if (!getSpecificObject(Player, objects).empty()) {
				doBotState(SEARCHING);
				break;
			}
			keepDown(hwndChild, 'A', 0.5);
			if (!getSpecificObject(Player, objects).empty()) {
				doBotState(SEARCHING);
				break;
			}
			keepDown(hwndChild, 'D', 1);
			if (!getSpecificObject(Player, objects).empty()) {
				doBotState(SEARCHING);
				break;
			}
			keepDown(hwndChild, 'S', 1);
			if (!getSpecificObject(Player, objects).empty()) {
				doBotState(SEARCHING);
				break;
			}
			else {
				std::cout << "Player could not be found! Sorry :( \n";
				break;
			}
		}

	
}
void askInput() {
	std::cout << "what action to take: \n";
	std::cout << "1. go to closest enemy \n";
	std::cout << "2. shoot closest enemy \n";
	double speed = 2.57;
	std::cin >> choice;
	if (choice == 1) {
		double tilePixelWidth = getWindowSize(hwndChild)[0] / tileWidth;
		double tilePixelHeight = getWindowSize(hwndChild)[1] / tileHeight;
		std::cout << "how many tiles between u and nearest enemy (x): " << std::to_string(getDistanceX(getSpecificObject(Player, objects)[0].position, getNearestEnemy(getSpecificObject(Player, objects)[0].position, objects)) / tilePixelWidth) << "\n";
		double time = (getDistanceX(getSpecificObject(Player, objects)[0].position, getNearestEnemy(getSpecificObject(Player, objects)[0].position, objects)) / tilePixelWidth) / speed;
		double time2 = (getDistanceY(getMid(getSpecificObject(Player, objects)[0].rect), getNearestEnemy(getMid(getSpecificObject(Player, objects)[0].rect), objects)) / tilePixelWidth) / speed;
		
		double tileDistanceX = (getDistanceX(getSpecificObject(Player, objects)[0].position, getNearestEnemy(getSpecificObject(Player, objects)[0].position, objects)) / tilePixelWidth) ;
		double tileDistanceY = (getDistanceY(getMid(getSpecificObject(Player, objects)[0].rect), getNearestEnemy(getMid(getSpecificObject(Player, objects)[0].rect), objects)) / tilePixelWidth);
		double tileDistanceDiagonal = sqrt(std::pow(tileDistanceX, 2) + std::pow(tileDistanceY, 2));
		std::cout << "x: " << std::to_string(tileDistanceX) << "\n";
		std::cout << "y: " << std::to_string(tileDistanceY) << "\n";
		double time3 = tileDistanceDiagonal / speed;
		char key = getAppropriateKeyX(getSpecificObject(Player, objects)[0].position, getNearestEnemy(getSpecificObject(Player, objects)[0].position, objects));
		char key2 = getAppropriateKeyY(getSpecificObject(Player, objects)[0].position, getNearestEnemy(getSpecificObject(Player, objects)[0].position, objects));

		if (abs(tileDistanceX - tileDistanceY) < 0.7) {
			std::cout << "Diagonal! \n";
			std::thread moveX(keepDown, hwndChild, key, 0.7 * time3);
			std::thread moveY(keepDown, hwndChild, key2,0.7 * time3);
			moveY.join();
			moveX.join();
		}
		else {
			std::cout << "not Diagonal! \n";
			keepDown(hwndChild, key,0.7 * time);
			keepDown(hwndChild, key2,0.7 * time2);
		}
		
		autoAim(hwndChild, 0);
		askInput();
		
	}
	 
	else if (choice == 2) {


		shootCoordinate(hwndChild, getNearestEnemy(getSpecificObject(Player, objects)[0].position, objects));
		
		askInput();
	}

	else if (choice  == 3) {
		
		double tilePixelWidth;
		double tilePixelHeight ;
		double time;
		double time2;
		double time3;
		double tileDistanceX;
		double tileDistanceY;
		double tileDistanceDiagonal;
		double key;
		double key2;
	
		std::thread mainLoop(doBotState, SEARCHING);
		mainLoop.join();
	}


	else {
		std::cout << "wrong."; 
		askInput();
	}
	askInput();
}

void movePlayer() {

	while (getDistance(getSpecificObject(Player, objects)[0].position, getNearestEnemy(getSpecificObject(Player, objects)[0].position, objects)) > 3) {

		std::cout << "x: " << std::to_string(getNearestEnemy(getSpecificObject(Player, objects)[0].position, objects).x) << std::endl;
		std::cout << "y: " << std::to_string(getNearestEnemy(getSpecificObject(Player, objects)[0].position, objects).y) << std::endl;
		std::cout << "x2: " << getAppropriateKeyX(getSpecificObject(Player, objects)[0].position, getNearestEnemy(getSpecificObject(Player, objects)[0].position, objects));
		std::cout << "y2: " << getAppropriateKeyY(getSpecificObject(Player, objects)[0].position, getNearestEnemy(getSpecificObject(Player, objects)[0].position, objects));
		clickKey(hwndChild, getAppropriateKeyX(getSpecificObject(Player, objects)[0].position, getNearestEnemy(getSpecificObject(Player, objects)[0].position, objects)), "down");
		clickKey(hwndChild, getAppropriateKeyY(getSpecificObject(Player, objects)[0].position, getNearestEnemy(getSpecificObject(Player, objects)[0].position, objects)), "down");
	}
	clickKey(hwndChild, getAppropriateKeyX(getSpecificObject(Player, objects)[0].position, getNearestEnemy(getSpecificObject(Player, objects)[0].position, objects)), "up");
	clickKey(hwndChild, getAppropriateKeyY(getSpecificObject(Player, objects)[0].position, getNearestEnemy(getSpecificObject(Player, objects)[0].position, objects)), "up");
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

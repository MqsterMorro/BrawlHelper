#include "Object.h"
#include <map>
#include <cmath>

Object::Object() {
}

Object::Object(cv::Rect rect, int x, int y, Name name) {

	this->rect = rect;
	this->position = cv::Point(x, y);
	this->name = name;
}


LPCWSTR windowTitleHere = L"BlueStacks App Player";
HWND hWNDHere = FindWindow(NULL, windowTitleHere);
HWND hwndChildHere = GetWindow(hWNDHere, GW_CHILD);

void drawEnemyLines(cv::Mat background, std::vector<Object> objects, cv::Point playerLocation) {


	std::vector<Object> enemies;
	for (size_t i = 0; i < objects.size(); i++) {
		if (objects[i].name == Enemy) {
			enemies.push_back(objects[i]);
		}
	}
	if (!(enemies.size() > 0)) {
		return;
	}
	cv::Point nearest;
	std::map<int, cv::Point> distances;
	std::vector<int> distancesInt;
	for (size_t i = 0; i < enemies.size(); i++) {
		int height = abs(enemies[i].position.y - playerLocation.y);
		int width = abs(enemies[i].position.x - playerLocation.x);
		int distance = sqrt(std::pow(height, 2) + std::pow(width, 2));
		distances.insert(std::pair<int, cv::Point>(distance, enemies[i].position));
	}
	// Create a map iterator and point to beginning of map
	std::map<int, cv::Point>::iterator it = distances.begin();
	while (it != distances.end())
	{
		distancesInt.push_back(it->first);
		it++;
	}
	int smallest = distancesInt[0];
	for (int i = 0; i < distancesInt.size(); i++) {
		if (distancesInt[i] < smallest) {
			smallest = distancesInt[i];
		}
	}


	nearest = distances.find(smallest)->second;
	cv::line(background, nearest, playerLocation, cv::Scalar(0, 255, 0), 1);

}

int getDistance(cv::Point one, cv::Point two) {
	int height = abs(one.y - two.y);
	int width = abs(one.x - two.x);
	std::cout << "abs distance is " << sqrt(std::pow(height, 2)+ std::pow(width, 2)) << std::endl;
	return sqrt(std::pow(height, 2) + std::pow(width, 2));
}

double getDistanceX(cv::Point one, cv::Point two) {
	int width = abs(one.x - two.x);
	return width;
}

double getDistanceY(cv::Point one, cv::Point two) {
	int height = abs(one.y - two.y);
	return height;
}

cv::Point getMid(cv::Rect rect) {
	return cv::Point(rect.x + rect.width , rect.y);
}

void drawObjects(cv::Mat background, std::vector<Object> objects) {
	int count = 0;
	Object first;
	for (size_t i = 0; i < objects.size(); i++) {
		switch (objects[i].name) {
		case Player:
			cv::putText(background, "Player", objects[i].rect.tl(), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(50, 50, 50));		
				rectangle(background, objects[i].rect.tl(), objects[i].rect.br(), CV_RGB(255, 0, 255), 2);
			
			drawEnemyLines(background, objects, getSpecificObject(Player, objects)[0].position);
			break;
		case Enemy:
			cv::putText(background, std::to_string(objects[i].position.x) + " " + std::to_string(objects[i].position.y), objects[i].rect.tl(), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255, 0, 0));
			rectangle(background, objects[i].rect.tl(), objects[i].rect.br(), CV_RGB(255, 0, 0), 2);
			break;
		case Bush:
			cv::putText(background, "Bush", objects[i].rect.tl(), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(50, 50, 50));
			rectangle(background, objects[i].rect.tl(), objects[i].rect.br(), CV_RGB(255, 0, 0), 2);
			break;
		case Ally:
			cv::putText(background, "Ally", objects[i].rect.tl(), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(50, 50, 50));
			rectangle(background, objects[i].rect.tl(), objects[i].rect.br(), CV_RGB(255, 0, 0), 2);
			break;
		default:
			cv::putText(background, "not found", objects[i].rect.tl(), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(50, 50, 50));
		}
	}

}


void getObjects(cv::Mat img, cv::Scalar low, cv::Scalar high, Name name, std::vector<Object>& objects) {
	cv::Mat mask;
	cv::inRange(img, low, high, mask);
	std::vector<std::vector<cv::Point> > contours;
	cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	for (size_t i = 0; i < contours.size(); i++)
	{
		cv::Rect boundRect = boundingRect(contours[i]);
		cv::Rect newRect;
		switch (name) {
		case Player:
			if ( getTilesX(hwndChildHere, boundRect.width) > 0.5 &&getTilesX(hwndChildHere, boundRect.width) < 2 &&  getTilesY(hwndChildHere, boundRect.height) < 3 && getTilesY(hwndChildHere, boundRect.height) > 0.5) {
				if (getSpecificObject(Player, objects).empty()) {
					objects.emplace_back(boundRect, boundRect.x + boundRect.width / 2, boundRect.y + boundRect.height / 2, name);
				}
				else {
					std::cout << "uh this isnt the 1st player here.. \n";
					std::cout << "size:  " << getSpecificObject(Player, objects).size() << " \n";
					if (boundRect.x > getSpecificObject(Player, objects)[0].position.x) {
						newRect.x = getSpecificObject(Player, objects)[0].position.x;
						newRect.width = boundRect.x - getSpecificObject(Player, objects)[0].position.x;
					}
					else {
						newRect.x = boundRect.x;
						newRect.width = getSpecificObject(Player, objects)[0].position.x - boundRect.x;
					}

					if (boundRect.y > getSpecificObject(Player, objects)[0].position.y) {
						newRect.y = getSpecificObject(Player, objects)[0].position.y;
						newRect.height = boundRect.y - getSpecificObject(Player, objects)[0].position.y;
					}
					else {
						newRect.y = boundRect.y;
						newRect.height = getSpecificObject(Player, objects)[0].position.y - boundRect.y;
					}
				
					int index;
					//remove the first player object
					for (size_t i = 0; i < objects.size(); i++) {
						if (objects[i].name == Player) {
							index = i;
							break;
						}
					}
					index = index - 1;
					auto elem_to_remove = objects.begin() + index;
					if (elem_to_remove != objects.end()) {
						objects.erase(elem_to_remove);
					}
					std::cout << "Removed successfully! \n";
						objects.emplace_back(newRect, newRect.x + newRect.width / 2, newRect.y + newRect.height / 2, name);
					
					
				}
	}
			break;
		case Enemy:
			if (0.4 * getTileArea(hwndChildHere) < boundRect.area() && getTilesX(hwndChildHere, boundRect.width) > 0.2 && getTilesX(hwndChildHere, boundRect.width) < 2 && getTilesY(hwndChildHere, boundRect.height) < 2 && getTilesY(hwndChildHere, boundRect.height) > 0.7) {
				//std::cout << std::abs(boundRect.width - boundRect.height) << std::endl;
					
						objects.emplace_back(boundRect, boundRect.x + boundRect.width / 2, boundRect.y + boundRect.height / 2, name);

					
				

					
				

			}
			
			break;
		case Bush:
			if (boundRect.area() > 2000 && (boundRect.width < 70 || boundRect.height < 70)) {
				objects.emplace_back(boundRect, boundRect.x + boundRect.width / 2, boundRect.y + boundRect.height / 2, name);
			}
			break;
		case Ally:
			if (0.4 * getTileArea(hwndChildHere) < boundRect.area() && getTilesX(hwndChildHere, boundRect.width) > 0.6 && (getTilesX(hwndChildHere, boundRect.width) < 4 || getTilesY(hwndChildHere, boundRect.height) < 4)) {
				objects.emplace_back(boundRect, boundRect.x + boundRect.width / 2, boundRect.y + boundRect.height / 2, name);
			}
			break;
		}

		

	}

}

std::vector<Object> getSpecificObject(Name name, std::vector<Object> objects) {

	std::vector<Object> theseObjects;

	for (size_t i = 0; i < objects.size(); i++) {
		if (objects[i].name == name) {
			theseObjects.push_back(objects[i]);
		}
	}

	if (theseObjects.empty()) {
		std::cout << "Didnt find any objects!" << std::endl;
		return theseObjects;
	}
	else {
		return theseObjects;
	}
}

char getAppropriateKeyX(cv::Point location, cv::Point destination) {

	if (location.x > destination.x) {
		return 'A';
	}
	else if (location.x < destination.x) {
		return 'D';
	}
	else {
		std::cout << "\n Same location! Weird \n";
		return 'U';
	}

}

char getAppropriateKeyY(cv::Point location, cv::Point destination) {

	if (location.y > destination.y) {
		return 'W';
	}
	else if (location.y < destination.y) {
		return 'S';
	}
	else {
		std::cout << "\n Same location! Weird \n";
		return 'U';
	}

}

cv::Point getNearestEnemy(cv::Point position, std::vector<Object> objects) {

	std::vector<Object> enemies;
	//get all enemies
	for (size_t i = 0; i < objects.size(); i++) {
		if (objects[i].name == Enemy) {
			enemies.push_back(objects[i]);
		}
	}
	//check  if there are any enemies / else return
	if (enemies.size() <= 0) {
		return cv::Point(0, 0);
	}

	cv::Point nearest;
	std::map<int, cv::Point> distances;
	std::vector<int> distancesInt;
	for (size_t i = 0; i < enemies.size(); i++) {
		int height = abs(enemies[i].position.y - position.y);
		int width = abs(enemies[i].position.x - position.x);
		int distance = sqrt(width + height);
		distances.insert(std::pair<int, cv::Point>(distance, getMid(enemies[i].rect)));
	}
	// Create a map iterator and point to beginning of map
	std::map<int, cv::Point>::iterator it = distances.begin();
	while (it != distances.end())
	{
		distancesInt.push_back(it->first);
		it++;
	}
	int smallest = distancesInt[0];
	for (int i = 0; i < distancesInt.size(); i++) {
		if (distancesInt[i] < smallest) {
			smallest = distancesInt[i];
		}
	}
	
	nearest = distances.find(smallest)->second;
	
	return nearest;
}

double getTilesX(HWND hwnd, int widthInPixels) {

	double tilePixelWidth = getWindowSize(hwnd)[0] / 24;
	return widthInPixels / tilePixelWidth;

}

double getTileWidth(HWND hwnd) {

	double tilePixelWidth = getWindowSize(hwnd)[0] / 24;
	return tilePixelWidth;

}

double getTilesY(HWND hwnd, int heightInPixels) {

	double tilePixelHeight = getWindowSize(hwnd)[0] / 17;
	return heightInPixels / tilePixelHeight;

}

double getTileArea(HWND hwnd) {

	double tilePixelWidth = getWindowSize(hwnd)[0] / 24;
	double tilePixelHeight = getWindowSize(hwnd)[0] / 17;
	return  tilePixelHeight * tilePixelHeight;

}
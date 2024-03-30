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
			cv::putText(background, std::to_string(int(objects[i].position.x)) + " " + std::to_string(int(objects[i].position.y)), objects[i].rect.tl(), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(50, 50, 50));
			count++;
			if (count >= 2) {
				if (250 > abs(first.rect.x - objects[i].rect.x)) {
					std::cout << "2 players!?";
					if (first.rect.tl().x < objects[i].rect.tl().x) {
						rectangle(background, first.rect.tl(), objects[i].rect.br(), CV_RGB(0, 255, 0), 2);
					}
					else {
						rectangle(background, objects[i].rect.tl(), first.rect.br(), CV_RGB(0, 255, 0), 2);
					}

				}
			}
			else {
				first = objects[i];
				rectangle(background, objects[i].rect.tl(), objects[i].rect.br(), CV_RGB(255, 0, 0), 2);
			}
			drawEnemyLines(background, objects, getSpecificObject(Player, objects).position);
			break;
		case Enemy:
			cv::putText(background, std::to_string(int(objects[i].position.x)) + " " + std::to_string(int(objects[i].position.y)), objects[i].rect.tl(), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(50, 50, 50));
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

		switch (name) {
		case Player:
			if (boundRect.area() > 500 && (boundRect.width < 70 || boundRect.height < 70)) {
				objects.emplace_back(boundRect, boundRect.x + boundRect.width / 2, boundRect.y + boundRect.height / 2, name);
			}
			break;
		case Enemy:
			if (boundRect.area() > 950 && (boundRect.width < 70 || boundRect.height < 70) && (30 > std::abs(boundRect.width - boundRect.height))) {
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
			if (boundRect.area() > 1050 && (boundRect.width < 70 || boundRect.height < 70)) {
				objects.emplace_back(boundRect, boundRect.x + boundRect.width / 2, boundRect.y + boundRect.height / 2, name);
			}
			break;
		}

	}

}

Object getSpecificObject(Name name, std::vector<Object> objects) {

	std::vector<Object> theseObjects;

	for (size_t i = 0; i < objects.size(); i++) {
		if (objects[i].name == name) {
			theseObjects.push_back(objects[i]);
		}
	}

	if (theseObjects.empty()) {
		std::cout << "Didnt find any objects!" << std::endl;
	}
	else {
		return theseObjects[0];
	}
}

char getAppropriateKeyX(cv::Point location, cv::Point destination) {

	if (location.x > destination.x) {
		return 'A';
	}
	else if (location.x < destination.x) {
		return 'D';
	}

}

char getAppropriateKeyY(cv::Point location, cv::Point destination) {

	if (location.y > destination.y) {
		return 'W';
	}
	else if (location.y < destination.y) {
		return 'S';
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
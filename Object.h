#pragma once
#include <opencv2/core/types.hpp>
#include "DarkHelp.hpp"
#include <iostream>


enum Name { Player, Enemy, Bush, Ally };

class Object {
public:

	cv::Point position;
	cv::Rect rect;
	Name name;
	Object();
	Object(cv::Rect rect, int x, int y, Name name);

};

void drawEnemyLines(cv::Mat, std::vector<Object>, cv::Point);
int getDistance(cv::Point, cv::Point);
void getObjects(cv::Mat, cv::Scalar, cv::Scalar, Name, std::vector<Object> &);
void drawObjects(cv::Mat, std::vector<Object>);
Object getSpecificObject(Name, std::vector<Object>);
char getAppropriateKeyX(cv::Point, cv::Point);
char getAppropriateKeyY(cv::Point, cv::Point);
cv::Point getNearestEnemy(cv::Point, std::vector<Object>);

#pragma once
#include <opencv2/core/types.hpp>

enum Name {Player, Enemy, Bush, Ally};
class Ball
{
public:

	cv::Point position;
	cv::Rect rect;
	Name name;
	Ball();
	Ball(cv::Rect rect, int x, int y, Name name);

};
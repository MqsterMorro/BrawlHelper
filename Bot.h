#pragma once
#include <Windows.h>
#include <opencv2/opencv.hpp>

enum BotState {
	JOIN_GAME, SEARCHING, WALKING, ATTACKING, EXIT
};
void aimAt(HWND, cv::Point, int);
void shootCoordinate(HWND, cv::Point);
void autoAim(HWND hwnd, int attack);

#pragma once
#include <Windows.h>
#include <opencv2/opencv.hpp>

void aimAt(HWND, cv::Point, int);
void shootCoordinate(HWND, cv::Point);
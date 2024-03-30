#pragma once
#include <Windows.h>
#include <opencv2/opencv.hpp>


cv::Mat getMat(HWND);
void clickKey(HWND, char, std::string);
void keepDown(HWND, char, double);
std::array<int,2> getWindowSize(HWND);
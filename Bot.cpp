#include "Bot.h"



void aimAt(HWND hwnd, cv::Point pos, int time) {
	PostMessage(hwnd, WM_ACTIVATE, WA_CLICKACTIVE, 0);
	PostMessage(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(pos.x, pos.y));
	Sleep(time);
	PostMessage(hwnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(pos.x, pos.y));
}

void shootCoordinate(HWND hwnd, cv::Point pos) {

	PostMessage(hwnd, WM_ACTIVATE, WA_CLICKACTIVE, 0);
	PostMessage(hwnd, WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(pos.x, pos.y - 50));
	Sleep(500);
	PostMessage(hwnd, WM_RBUTTONUP, MK_RBUTTON, MAKELPARAM(pos.x, pos.y - 50));
}

void autoAim(HWND hwnd, int attack) {

	char key;

	if (attack == 0) {
		key = 'R';
	}
	else {
		key = 'G';
	}
	PostMessage(hwnd, WM_ACTIVATE, WA_CLICKACTIVE, 0);
	PostMessage(hwnd, WM_KEYDOWN, key, 0);
	Sleep(50);
	PostMessage(hwnd, WM_KEYUP, key, 0);
}
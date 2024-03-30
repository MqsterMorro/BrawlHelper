#include "WindowUtils.h"

cv::Mat getMat(HWND hWND) {
	//copied code 
	HDC deviceContext = GetDC(hWND);
	HDC memoryDeviceContext = CreateCompatibleDC(deviceContext);

	RECT windowRect;
	GetClientRect(hWND, &windowRect);

	int height = windowRect.bottom;
	int width = windowRect.right;

	HBITMAP bitmap = CreateCompatibleBitmap(deviceContext, width, height);

	SelectObject(memoryDeviceContext, bitmap);

	//copy data into bitmap
	BitBlt(memoryDeviceContext, 0, 0, width, height, deviceContext, 0, 0, SRCCOPY);


	//specify format by using bitmapinfoheader!
	BITMAPINFOHEADER bi;
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = width;
	bi.biHeight = -height;
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0; //because no compression
	bi.biXPelsPerMeter = 1; //we
	bi.biYPelsPerMeter = 2; //we
	bi.biClrUsed = 3; //we ^^
	bi.biClrImportant = 4; //still we

	cv::Mat mat = cv::Mat(height, width, CV_8UC4); // 8 bit unsigned ints 4 Channels -> RGBA

	//transform data and store into mat.data
	GetDIBits(memoryDeviceContext, bitmap, 0, height, mat.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

	//clean up!
	DeleteObject(bitmap);
	DeleteDC(memoryDeviceContext); //delete not release!
	ReleaseDC(hWND, deviceContext);

	return mat;
}

void clickKey(HWND hwndChild, char key, std::string type) {

	SendMessage(hwndChild, WM_ACTIVATE, WA_CLICKACTIVE, 0);
	if (type == "down") {
		PostMessage(hwndChild, WM_KEYDOWN, key, 0);
	}
	else {
		PostMessage(hwndChild, WM_KEYUP, key, 0);
	}

}

void keepDown(HWND hwndChild, char key, double time) {

	SendMessage(hwndChild, WM_ACTIVATE, WA_CLICKACTIVE, 0);
	
		PostMessage(hwndChild, WM_KEYDOWN, key, 0);
		Sleep(time * 1000);
		PostMessage(hwndChild, WM_KEYUP, key, 0);
	
	

}

std::array<int, 2> getWindowSize(HWND hwnd) {
	RECT windowRect;
	GetClientRect(hwnd, &windowRect);
	std::array<int,2> size;
	size[0] = windowRect.right;
	size[1] = windowRect.bottom - 35;
	return size;
}
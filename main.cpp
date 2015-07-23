#include "MouseDetector.h"
#include <time.h>

cv::Mat image;
MouseDetector MouseGlove;

cv::Point coordenadasMouse;

cv::VideoCapture camera(0);

bool leftClickStatus;
bool rightClickStatus;

clock_t t;

float getTime() {
	return float(clock() - t)/CLOCKS_PER_SEC;
}

void resetTimer() {
	t = clock();
}

void toggleLeftClick() {
	leftClickStatus = MouseGlove.getLeftClickStatus();
	if (leftClickStatus == true) MouseGlove.leftClickDown(coordenadasMouse);
	else MouseGlove.leftClickUp(coordenadasMouse);
	resetTimer();
}

void toggleRightClick() {
	rightClickStatus = MouseGlove.getRightClickStatus();
	if (rightClickStatus == true) MouseGlove.rightClickDown(coordenadasMouse);
	else MouseGlove.rightClickUp(coordenadasMouse);
	resetTimer();
}

int main() {
    if (!camera.isOpened()) return 1;
	int keyCheck = 0;
	resetTimer();
	MouseGlove.setCenterHSV(140,161,145);
	MouseGlove.setLeftHSV(96,68,118);
	MouseGlove.setRightHSV(38,205,246);
	MouseGlove.setScrollHSV(63,144,204);
	MouseGlove.setCenterColorThreshold(45);
	MouseGlove.setLeftColorThreshold(0);
	MouseGlove.setRightColorThreshold(25);
	MouseGlove.setScrollColorThreshold(0);
	MouseGlove.setScrollAnchorYCoordinate(240);
	MouseGlove.setMinArea(100);
	leftClickStatus = false;
	rightClickStatus = false;
    while(cv::waitKey(10) != 13) {
		if (!camera.read(image)) return 1;
		cv::flip(image,image,1);
		MouseGlove.processCenterMarker(image);
		MouseGlove.processLeftMarker(image);
		MouseGlove.processRightMarker(image);
		MouseGlove.processScrollMarker(image);
		if (getTime() > 0.3) {
			coordenadasMouse = MouseGlove.getCenterMarkerCoordinates();
			MouseGlove.calibrateCoordinates(coordenadasMouse);
		}
		if (MouseGlove.mouseDetected()) {
			MouseGlove.moveMouse(coordenadasMouse);
	//	}
		//if (MouseGlove.getLeftClickStatus() != leftClickStatus)
		//	toggleLeftClick();
		//if (MouseGlove.getRightClickStatus() != rightClickStatus)
		//	toggleRightClick();
	//	if (MouseGlove.scrollDetected()) {
		//	MouseGlove.scroll(coordenadasMouse,MouseGlove.getScrollSpeed());
	//	}
		MouseGlove.displayMouseStatus(image);
		cv::imshow("Mouse",image);
    }
}

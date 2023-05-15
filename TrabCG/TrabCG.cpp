#include <iostream>
#include <opencv2/opencv.hpp>
#include "Image.hpp"
#include "Polygon.h"
#include "PolygonFunctions.h"

using namespace cv;
using namespace std;
using namespace lib;


int main(int argc, char** argv)
{
	int width = 500;
	int height = 500;
	lib::Point center = lib::Point(width * 0.50, height * 0.50);
	String windowName = "Window";
	//lib::Polygon* triangle = lib::createTriangle(300, center, {Color::blue, Color::salmon, Color::orange});
	auto polygon = createSquare(400, center, { Color::blue , Color::magenta, Color::cyan, Color::yellow});
	auto background = createZerosMat(500, 500);
	polygon->Draw(background);
	namedWindow(windowName); // Create a window

	imshow(windowName, *background);

	waitKey(0); // Wait for any keystroke in the window

	destroyWindow(windowName); //destroy the created window
	
	delete(polygon);
	delete(background);
	return 0;
}
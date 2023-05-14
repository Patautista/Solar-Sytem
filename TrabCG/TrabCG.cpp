#include <iostream>
#include <opencv2/opencv.hpp>
#include "Image.hpp"
#include "Polygon.h"
#include "PolygonFunctions.h"

using namespace cv;
using namespace std;


int main(int argc, char** argv)
{
	int width = 500;
	int height = 500;
	String windowName = "Window";

	lib::Polygon* square = lib::createSquare(300, lib::Point(width * 0.50, height * 0.50));
	lib::Polygon* circle = lib::createCircle(200, lib::Point(width * 0.50, height * 0.50));
	Mat* background = createZerosMat(500, 500);
	circle->Draw(background);
	namedWindow(windowName); // Create a window

	imshow(windowName, *background);

	waitKey(0); // Wait for any keystroke in the window

	destroyWindow(windowName); //destroy the created window
	
	delete(square);
	delete(circle);
	delete(background);
	return 0;
}
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
	String windowName = "Window";
	namedWindow(windowName); // Create a window
	auto center = lib::Point(width * 0.50, height * 0.50);

	//auto polygon = createTriangle(300, center, {Color::blue, Color::salmon, Color::orange});
	auto polygon = createSquare(250, center, {Color::blue , Color::magenta, Color::cyan, Color::yellow});
	auto background = createZerosMat(500, 500);
	while (true) {
		polygon->Draw(background);
		imshow(windowName, *background);
		background = createZerosMat(500, 500);
		polygon->Rotate(45, center);
		waitKey(1000); // Wait for any keystroke in the window
	}

	destroyWindow(windowName); //destroy the created window
	
	delete(polygon);
	delete(background);
	return 0;
}
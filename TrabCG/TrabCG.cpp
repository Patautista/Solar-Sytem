#include <iostream>
#include <opencv2/opencv.hpp>
#include "Image.hpp"
#include "Polygon.h"
#include "PolygonFunctions.h"
#include "PolygonIntersection.h"


using namespace cv;
using namespace std;
using namespace lib;


int main(int argc, char** argv)
{ 
	int viewport_width = 500;
	int viewport_height = 500;
	String windowName = "Window";
	vector<Polygon*> polygons;
	auto center = lib::Point(viewport_width * 0.50, viewport_height * 0.50);
	auto square = createSquare(250, center, { Color::blue , Color::magenta, Color::cyan, Color::yellow });
	auto circle = createCircle(viewport_height * 0.25, center, 420, Color::blue);
	polygons.push_back(square);
	polygons.push_back(circle);
	namedWindow(windowName); // Create a window
	for (auto& poly:polygons) {
		vector<Polygon*> polygons;
	}
	//auto polygon = createTriangle(500, center, {Color::blue, Color::salmon, Color::orange});
	auto window = createRectangle(
		lib::Point(viewport_width * 0.10, viewport_height *0.60, Color::brown),
		lib::Point(viewport_width * 0.80, viewport_height * 0.20, Color::brown));
	auto polygon = findIntersection(circle, window);
	auto viewport = createZerosMat(viewport_height, viewport_width);
	auto texture = cv::imread("./textures/tex1.png");
	//window->Draw(viewport);
	polygon->DrawTexture(&viewport, &texture);
	//square->Draw(viewport);
	imshow(windowName, viewport);
	waitKey(0);

	destroyWindow(windowName); //destroy the created window
	
	delete(polygon);
	return 0;
}
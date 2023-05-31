#include <iostream>
#include <opencv2/opencv.hpp>
#include "Image.hpp"
#include "Polygon.h"
#include "PolygonFunctions.h"
#include "PolygonIntersection.h"
#include "Window.h"
#include "Windows.h"


using namespace cv;
using namespace std;
using namespace lib;

std::atomic<bool> stopFlag(false);

void rotateThread(lib::Polygon* poly, double angle, lib::Point center)
{
	while (!stopFlag)
	{
		poly->Rotate(angle, center);
		poly->Rotate(angle, poly->mCenter);
		Sleep(100);
	}
	std::cout << "Thread stopped" << std::endl;
}

int main(int argc, char** argv)
{ 
	// Define viewport dimensions
	auto center = lib::Point(0, 0);
	int viewport_width = 1100;
	int viewport_height = 800;
	auto viewport = createZerosMat(viewport_height, viewport_width);
	// Define window attributes
	lib::Point window_top_left = lib::Point(-250, 250);
	lib::Point window_bottom_right = lib::Point(100, -250);
	String windowName = "Window";
	namedWindow(windowName); // Create a window
	auto window = Window(window_top_left, window_bottom_right);
	
	// Create polygons set
	vector<lib::Polygon*> polygons;
	auto texture = cv::imread("./textures/background.jpg");
	//auto circle = createCircle(viewport_height * 0.25, center, 30, Color::blue);

	// Sun
	auto sun = createOctagon(center, 70,
		{ Color::red , Color::orange, Color::yellow, Color::orange, Color::white, Color::red, Color::yellow,  Color::orange }
	);
	polygons.push_back(sun);
	// Mercury
	auto mercury = createOctagon(lib::Point(-120, 40), 20,
		{ Color::pastelRed , Color::gray, Color::cream, Color::pastelRed, Color::cream, Color::brown, Color::darkRed,  Color::gray }
	);
	polygons.push_back(mercury);
	// Venus
	auto venus = createOctagon(lib::Point(-160, 120), 40,
		{ Color::orange , Color::gray, Color::darkRed, Color::gray, Color::brown, Color::orange, Color::brown,  Color::black }
	);
	polygons.push_back(venus);
	// Earth :D
	auto earth = createOctagon(lib::Point(-250, 200), 40,
		{ Color::blue , Color::gray, Color::darkGreen, Color::blue, Color::green, Color::white, Color::brown,  Color::darkBlue }
	);
	polygons.push_back(earth);
	// Mars
	auto mars = createOctagon(lib::Point(-340, 218), 28,
		{ Color::darkRed , Color::red, Color::brown, Color::orange , Color::black, Color::brown, Color::red,  Color::darkYellow }
	);
	polygons.push_back(mars);
	// Jupiter
	auto jupiter = createOctagon(lib::Point(-600, 420), 60,
		{ Color::cream , Color::brown, Color::pastelRed, Color::cream , Color::brown, Color::pastelRed, Color::cream,  Color::brown }
	);
	polygons.push_back(jupiter);
	// Saturn
	auto saturn = createOctagon(lib::Point(-900, 420), 60,
		{ Color::cream , Color::brown, Color::pastelRed, Color::cream , Color::brown, Color::pastelRed, Color::cream,  Color::brown }
	);
	saturn->setTexture(imread("./textures/saturn.jpg"));
	polygons.push_back(saturn);
	// Uran
	auto uran = createOctagon(lib::Point(-1100, 1400), 47,
		{ Color::pastelBlue , Color::gray, Color::pastelBlue, Color::cyan , Color::gray, Color::blue, Color::cyan,  Color::gray }
	);
	polygons.push_back(uran);

	window.UpdateVisiblePolygons(polygons);
	window.Zoom(3);

	// Start everything and here we go
	std::thread rotate0(std::bind(rotateThread,polygons[0], 8, center));
	std::thread rotate1(std::bind(rotateThread, polygons[1], -5.4, center));
	std::thread rotate2(std::bind(rotateThread, polygons[2], -6, center));
	std::thread rotate3(std::bind(rotateThread, polygons[3], -4, center));
	std::thread rotate4(std::bind(rotateThread, polygons[4], -7, center));
	std::thread rotate5(std::bind(rotateThread, polygons[5], -7, center));
	std::thread rotate6(std::bind(rotateThread, polygons[6], -10, center));
	std::thread rotate7(std::bind(rotateThread, polygons[7], -6.8, center));

	while (true) {
		if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
			window.Translate(-50, 0);
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
			window.Translate(50, 0);
		}
		if (GetAsyncKeyState(VK_UP) & 0x8000) {
			window.Translate(0, 50);
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
			window.Translate(0, -50);
		}
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			break;
		}
		if (GetAsyncKeyState('A') & 0x8000) {
			window.Zoom(2);
		}
		if (GetAsyncKeyState('S') & 0x8000) {
			window.Zoom(0.5);
		}
		window.Draw(&viewport);
		imshow(windowName, viewport);
		viewport = createZerosMat(viewport_height, viewport_width);
		waitKey(50);
	}
	stopFlag = true;
	rotate0.join();
	rotate1.join();
	rotate2.join();
	rotate3.join();
	rotate4.join();
	rotate5.join();
	rotate6.join();
	rotate7.join();
	cv::destroyWindow(windowName); //destroy the created window
	
	for (auto poly : polygons) {
		delete(poly);
	}

	return 0;
}
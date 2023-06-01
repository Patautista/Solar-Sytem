#include <iostream>
#include <opencv2/opencv.hpp>
#include "Image.hpp"
#include "Polygon.h"
#include "PolygonFunctions.h"
#include "PolygonIntersection.h"
#include "Window.h"
#include "Windows.h"

#pragma comment(lib, "winmm.lib")


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
void scaleThread(lib::Polygon* poly, lib::Point center)
{
	while (!stopFlag)
	{
		for (int i = 0; i <= 7; i++) {
			poly->Scale(1.02, 1.02, center);
			Sleep(100);
		}
		for (int i = 0; i <= 7; i++) {
			poly->Scale(0.98, 0.98, center);
			Sleep(100);
		}
	}
	std::cout << "Thread stopped" << std::endl;
}

void showInitialScreen(Mat& screen)
{	
	cv::Mat earth_text = imread("./textures/earth.png");
	drawCircle(screen, screen.cols / 4, screen.rows * 3 / 4 + 250, 325, cv::Vec3b(1, 1, 1));
	lib::floodFillWithMatrix(screen, earth_text, cv::Point(screen.cols / 4 + 1, screen.rows * 3 / 4 + 100));

	//Draw static with flood fill saturn planet
	cv::Mat sat_text = imread("./textures/saturntext.png");
	drawCircle(screen, screen.cols - 100, screen.rows/4, 25, cv::Vec3b(1, 1, 1));
	lib::floodFill(screen, screen.rows / 4, screen.cols - 100, cv::Vec3b(191, 229, 250));

	//Draw static with flood fill saturn ring
	cv::Mat satring_text = imread("./textures/saturnring.png");
	drawEllipse(screen, cv::Point(screen.cols - 100, screen.rows / 4), 125, 20, cv::Vec3b(1, 1, 1), 1);
	lib::floodFill(screen, screen.rows / 4, screen.cols - 150, cv::Vec3b(175, 189, 191));
	lib::floodFill(screen, screen.rows / 4, screen.cols - 50, cv::Vec3b(175, 189, 191));
	
	
	//Draw static with flood fill Sun
	cv::Mat sun_text = imread("./textures/sun.png");
	drawCircle(screen, screen.cols / 2, screen.rows / 2 - 20, 131 , cv::Vec3b(1,1,1));
	lib::floodFillWithMatrix(screen, sun_text, cv::Point(screen.cols / 2, screen.rows / 2));
	
}

int main(int argc, char** argv)
{ 
	// Define viewport dimensions
	auto center = lib::Point(0, 0);
	int viewport_width = 1000;
	int viewport_height = 800;
	Mat viewport = createZerosMat(viewport_height, viewport_width);
	// Define window attributes
	lib::Point window_top_left = lib::Point(-250, 250);
	lib::Point window_bottom_right = lib::Point(100, -250);
	String windowName = "Window";
	namedWindow(windowName); // Create a window
	auto window = Window(window_top_left, window_bottom_right);
	
	// Create polygons set
	vector<lib::Polygon*> polygons;
	Mat texture = cv::imread("./textures/background.png");
	
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
	TruncateTexture(viewport, texture);
	showInitialScreen(viewport);
	imshow(windowName, viewport);
	PlaySound(TEXT("path/to/your/sound/file.wav"), NULL, SND_ASYNC | SND_LOOP);
	waitKey(0);
	
	// Start everything and here we go
	std::thread scale0(std::bind(scaleThread,polygons[0], center));
	std::thread rotate1(std::bind(rotateThread, polygons[1], -15, center));
	std::thread rotate2(std::bind(rotateThread, polygons[2], -5, center));
	std::thread rotate3(std::bind(rotateThread, polygons[3], -4, center));
	std::thread rotate4(std::bind(rotateThread, polygons[4], -7, center));
	std::thread rotate5(std::bind(rotateThread, polygons[5], -4.5, center));
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
		TruncateTexture(viewport, texture);
		waitKey(50);
	}
	stopFlag = true;
	PlaySound(NULL, NULL, 0);
	scale0.join();
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
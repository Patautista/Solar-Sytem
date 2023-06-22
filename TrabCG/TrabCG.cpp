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

	auto tilte = createSquare(screen.cols * 0.10, lib::Point(screen.cols * 0.20, screen.rows * 0.15), {});
	tilte->setTexture(imread("./textures/title.jpg"));
	tilte->Scale(3, 1, tilte->mCenter);
	tilte->Draw(&screen);

	auto credits = createSquare(screen.cols * 0.10, lib::Point(screen.cols * 0.20, screen.rows * 0.30), {});
	credits->setTexture(imread("./textures/credits.png"));
	credits->Scale(3.5, 0.8, credits->mCenter);
	credits->Draw(&screen);
	
	auto instructions = createSquare(screen.cols*0.17, lib::Point(screen.cols * 0.80, screen.rows * 0.80), {});
	instructions->setTexture(imread("./textures/instructions.jpg"));
	instructions->Scale(2, 0.94, instructions->mCenter);
	instructions->Draw(&screen);
	delete(instructions);
	delete(credits);
	delete(tilte);
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
	auto square = createSquare(400, center, {Color::red, Color::red ,Color::red,Color::red});

	polygons.push_back(square);

	window.UpdateVisiblePolygons(polygons);
	window.Zoom(3);
	TruncateTexture(viewport, texture);
	showInitialScreen(viewport);
	imshow(windowName, viewport);
	PlaySound(TEXT("./sound/intro.wav"), NULL, SND_ASYNC | SND_LOOP);
	waitKey(0);
	PlaySound(TEXT("./sound/sobre_as_estrelas.wav"), NULL, SND_ASYNC | SND_LOOP);
	

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

	cv::destroyWindow(windowName); //destroy the created window
	
	for (auto poly : polygons) {
		delete(poly);
	}

	return 0;
}
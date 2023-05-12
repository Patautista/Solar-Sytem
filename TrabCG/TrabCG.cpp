#include <iostream>
#include <opencv2/opencv.hpp>
#include "Image.hpp"
#include "Polygon.hpp"

using namespace cv;
using namespace std;


int main(int argc, char** argv)
{
	// Read the image file
	Polygon* square = new Polygon();
	Mat* zeroImg = createZerosMat(500, 500);
	drawLineDDA(zeroImg, 100, 60, 200, 120);


	String windowName = "Héricles"; //Name of the window

	namedWindow(windowName); // Create a window

	imshow(windowName, *zeroImg); // Show our image inside the created window.

	waitKey(0); // Wait for any keystroke in the window

	destroyWindow(windowName); //destroy the created window
	
	delete(zeroImg);
	return 0;
}

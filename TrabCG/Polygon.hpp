#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include "Drawing Functions.h"
class Polygon {
private:
    std::vector<cv::Point> points;

public:
    Polygon(const std::vector<cv::Point>& _points) : points(_points) {}

    void Draw(cv::Mat* mat) {
        int n = points.size();

        for (int i = 0; i < n; i++) {
            int j = (i + 1) % n;
            drawLineDDA(mat, points[i].x, points[i].y, points[j].x, points[j].y);
        }
    }
};
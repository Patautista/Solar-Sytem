#pragma once
#include <iostream>
#include "Point.h"
#include "Drawing Functions.h"
namespace lib {
    class Polygon {
    private:
        std::vector<Point> points;
        Polygon* boundingBox;

    public:
        Polygon(const std::vector<Point>& _points) : points(_points) {}

        void Draw(cv::Mat* mat) {
            int n = points.size();

            for (int i = 0; i < n; i++) {
                int j = (i + 1) % n;
                drawLineDDA(mat, points[i].x, points[i].y, points[j].x, points[j].y);
            }
        }
        void setPoints(std::vector<Point> newPoints) {
            points = newPoints;
            updateBoundingBox();
        }
        std::vector<Point> getPoints() const {
            return points;
        }
        void updateBoundingBox() {
            int minX = INT_MAX, minY = INT_MAX;
            int maxX = INT_MIN, maxY = INT_MIN;

            for (Point point : points) {
                if (point.x < minX) {
                    minX = point.x;
                }
                if (point.x > maxX) {
                    maxX = point.x;
                }
                if (point.y < minY) {
                    minY = point.y;
                }
                if (point.y > maxY) {
                    maxY = point.y;
                }
            }
            //boundingBox.setPoints({ cv::Point(minX, minY), cv::Point(maxX, minY), cv::Point(maxX, maxY), cv::Point(minX, maxY) });
        }
    };
}
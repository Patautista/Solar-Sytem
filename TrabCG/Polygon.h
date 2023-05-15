#pragma once
#include <iostream>
#include "Point.h"
#include "Drawing Functions.h"
namespace lib {
    class Polygon {
    private:
        std::vector<Point> points;
        Polygon* boundingBox;
        void scanline(const std::vector<Point>& points, cv::Mat* background) {
            // sort the points based on their y value
            std::vector<Point> sortedPoints = points;
            std::sort(sortedPoints.begin(), sortedPoints.end(),
                [](const Point& a, const Point& b) { return a.y < b.y; });

            // create a list of x values for each height
            std::vector<std::vector<Point>> pointLists;
            int maxY = sortedPoints.back().y;
            pointLists.resize(maxY + 1);
            for (const auto& point : sortedPoints) {
                pointLists[point.y].push_back(point);
            }

            // draw lines between the points for each height
            for (const auto& list : pointLists) {
                if (list.size() > 1) {
                    for (int i = 0; i < list.size() - 1; ++i) {
                        Point x1 = list[i];
                        Point x2 = list[i + 1];
                        drawLineDDA(background, x1, x2);
                    }
                }
            }
        }
        void drawLineDDA(cv::Mat* mat, Point p1, Point p2) {
            int dx = p2.x - p1.x;
            int dy = p2.y - p1.y;
            int steps = std::max(std::abs(dx), std::abs(dy));
            float xIncrement = dx / (float)steps;
            float yIncrement = dy / (float)steps;
            // color increments
            float rIncrement = (p2.color.r - p1.color.r) / (float)steps;
            float gIncrement = (p2.color.g - p1.color.g) / (float)steps;
            float bIncrement = (p2.color.b - p1.color.b) / (float)steps;

            float x = p1.x;
            float y = p1.y;
            float r = p1.color.r;
            float g = p1.color.g;
            float b = p1.color.b;
            for (int i = 0; i <= steps; i++) {
                Point p((int)round(x), (int)round(y), Color((int)round(r), (int)round(g), (int)round(b)));
                mat->at<cv::Vec3b>(p.y, p.x) = cv::Vec3b(p.color.b, p.color.g, p.color.r);
                x += xIncrement;
                y += yIncrement;
                r += rIncrement;
                g += gIncrement;
                b += bIncrement;
                points.push_back(p);
            }
        }
    public:
        Polygon(const std::vector<Point>& _points) : points(_points) {}


        void Draw(cv::Mat* mat) {
            int n = points.size();

            for (int i = 0; i < n; i++) {
                int j = (i + 1) % n;
                drawLineDDA(mat, points[i], points[j]);
            }
            scanline(points, mat);
        }
        void DrawFrame(cv::Mat* mat) {
            int n = points.size();

            for (int i = 0; i < n; i++) {
                int j = (i + 1) % n;
                drawLineDDA(mat, points[i], points[j]);
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
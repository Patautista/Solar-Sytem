#pragma once
#include <iostream>
#include "Point.h"
#include "Drawing Functions.h"
namespace lib {
    class Polygon {
    private:
        bool m_framed;
        std::vector<Point> m_vertices;
        // used by scanline
        std::vector<Point> m_frame_points;
        Polygon* m_boundingBox;
        void _scanline(const std::vector<Point>& points, cv::Mat* background) {
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
                        _drawLineDDA(background, x1, x2);
                    }
                }
            }
        }
        void _drawLineDDA(cv::Mat* mat, Point p1, Point p2) {
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
                if(mat != nullptr) {
                    mat->at<cv::Vec3b>(p.y, p.x) = cv::Vec3b(p.color.b, p.color.g, p.color.r);
                }
                x += xIncrement;
                y += yIncrement;
                r += rIncrement;
                g += gIncrement;
                b += bIncrement;
                if (!m_framed) {
                    // add points to frame
                    m_frame_points.push_back(p);
                }
            }
        }
        void _transformPolygon(const cv::Mat& transform, const Point& center)
        {
            // Iterate through each point in the polygon
            for (auto& point : m_vertices)
            {
                // Translate the point so that the center is the origin
                point.x -= center.x;
                point.y -= center.y;

                // Apply the transformation matrix to the point
                cv::Mat pointMat = (cv::Mat_<double>(3, 1) << point.x, point.y, 1);
                cv::Mat transformedPointMat = transform * pointMat;

                // Update the point with the transformed coordinates
                point.x = transformedPointMat.at<double>(0, 0);
                point.y = transformedPointMat.at<double>(1, 0);

                // Translate the point back to its original position
                point.x += center.x;
                point.y += center.y;
            }

            // Update the bounding box of the polygon
            this->updateBoundingBox();
        }
        void _drawFrame(cv::Mat* mat) {
            int n = m_vertices.size();

            for (int i = 0; i < n; i++) {
                int j = (i + 1) % n;
                _drawLineDDA(mat, m_vertices[i], m_vertices[j]);
            }
            m_framed = true;
        }
    public:
        Polygon(const std::vector<Point>& _points) : m_vertices(_points), m_framed(false), m_frame_points(_points) {
        }

        void Draw(cv::Mat* mat) {
            this->_drawFrame(mat);
            //_scanline(m_frame_points, mat);
            m_frame_points = m_vertices;
            m_framed = false;
        }
        void Erase(cv::Mat* mat) {
            int n = m_vertices.size();

            for (int i = 0; i < n; i++) {
                mat->at<cv::Vec3b>(m_vertices[i].y, m_vertices[i].x) = cv::Vec3b(0, 0, 0);
            }
        }
        void Rotate(double angle, Point point) {
            double radians = angle * CV_PI / 180.0;
            double cosTheta = cos(radians);
            double sinTheta = sin(radians);

            cv::Mat rotationMatrix = (cv::Mat_<double>(3, 3) <<
                cosTheta, -sinTheta, 0,
                sinTheta, cosTheta, 0,
                0, 0, 1);
            this->_transformPolygon(rotationMatrix, point);
        }
        void Scale(double xfactor, double yfactor, Point point) {

            cv::Mat scaling = (cv::Mat_<double>(3, 3) << xfactor, 0, 0,
                0, yfactor, 0,
                0, 0, 1);
            this->_transformPolygon(scaling, point);
        }
        void Translate(double xfactor, double yfactor, Point point) {

            cv::Mat translation = (cv::Mat_<double>(3, 3) << 1, 0, xfactor,
                0, 1, yfactor,
                0, 0, 1);
            this->_transformPolygon(translation, point);
        }
        void setPoints(std::vector<Point> newPoints) {
            m_vertices = newPoints;
            updateBoundingBox();
        }
        std::vector<Point> getPoints() const {
            return m_vertices;
        }
        void updateBoundingBox() {
            int minX = INT_MAX, minY = INT_MAX;
            int maxX = INT_MIN, maxY = INT_MIN;

            for (Point point : m_vertices) {
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
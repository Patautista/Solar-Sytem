#pragma once
#include <iostream>
#include "Point.h"
#include "Drawing Functions.h"
namespace lib {
    class Polygon {
    private:
        bool m_framed;
        cv::Mat m_texture;
        std::vector<Point> m_vertices;
        // used by scanline
        std::vector<Point> m_frame_points;
        void _scanline(const std::vector<Point>& points, cv::Mat* background, cv::Mat* texture) {
            // sort the points based on their y value
            std::vector<Point> sortedPoints = points;
            std::sort(sortedPoints.begin(), sortedPoints.end(),
                [](const Point& a, const Point& b) { return a.y < b.y; });

            // create a list of x values for each height
            std::vector<std::vector<Point>> pointLists;
            int maxY = sortedPoints.back().y;
            pointLists.resize(maxY + 1);
            for (auto& point : sortedPoints) {
                if (point.y < 0 ) {
                    pointLists[0].push_back(point);
                }
                else {
                    pointLists[point.y].push_back(point);
                }
            }

            // draw lines between the points for each height
            for (const auto& list : pointLists) {
                if (list.size() > 1) {
                    for (int i = 0; i < list.size() - 1; ++i) {
                        Point p1 = list[i];
                        Point p2 = list[i + 1];
                        _drawLineDDA(background, texture, p1, p2);
                    }
                }
            }
            if (texture != nullptr) {
                // Find the minimum and maximum x and y coordinates
                double minX = std::numeric_limits<double>::max();
                double minY = std::numeric_limits<double>::max();
                double maxX = std::numeric_limits<double>::lowest();
                double maxY = std::numeric_limits<double>::lowest();

                for (const Point& point : this->getPoints()) {
                    if (point.x < minX) minX = point.x;
                    if (point.y < minY) minY = point.y;
                    if (point.x > maxX) maxX = point.x;
                    if (point.y > maxY) maxY = point.y;
                }
                // Compute the range of x and y coordinates
                double rangeX = maxX - minX;
                double rangeY = maxY - minY;

                // Normalize the points within the range [0, 1]
                for (Point& point : this->getPoints()) {
                    point.x = (point.x - minX) / rangeX;
                    point.y = (point.y - minY) / rangeY;
                }
                // draw lines between the points for each height
                for (const auto& list : pointLists) {
                    if (list.size() > 1) {
                        for (int i = 0; i < list.size() - 1; ++i) {
                            Point p1 = list[i];
                            Point p2 = list[i + 1];
                            if (p1.x < 0) {
                                p1.x = 0;
                            }
                            if (p1.y < 0) {
                                p1.y = 0;
                            }
                            if (p2.x < 0) {
                                p2.x = 0;
                            }
                            if (p2.y < 0) {
                                p2.y = 0;
                            }
                            int dx = p2.x - p1.x;
                            int dy = p2.y - p1.y;
                            float x = p1.x;
                            float y = p1.y;

                            int steps = std::max(std::abs(dx), std::abs(dy));
                            // cordinate increments
                            float xIncrement = dx / (float)steps;
                            float yIncrement = dy / (float)steps;
                            for (int i = 0; i <= steps; i++) {

                                Point p((int)round(x), (int)round(y), mapPointInTexture((x - minX) / rangeX, (y - minY) / rangeY, texture));
                                if (background != nullptr && (x < background->cols && y < background->rows)) {
                                    try {
                                        background->at<cv::Vec3b>(p.y, p.x) = cv::Vec3b(p.color.b, p.color.g, p.color.r);
                                    }
                                    catch (std::exception& ex) {

                                    }
                                }
                                if (!m_framed) {
                                    // add points to frame
                                    m_frame_points.push_back(p);
                                }
                                x += xIncrement;
                                y += yIncrement;
                            }   
                        }
                    }
                }
            }
        }
        void _drawLineDDA(cv::Mat* mat, cv::Mat* tex, Point p1, Point p2) {
            if (p1.x < 0) {
                p1.x = 0;
            }
            if (p1.y < 0) {
                p1.y = 0;
            }
            if (p2.x < 0) {
                p2.x = 0;
            }
            if (p2.y < 0) {
                p2.y = 0;
            }
            int dx = p2.x - p1.x;
            int dy = p2.y - p1.y;
            int steps = std::max(std::abs(dx), std::abs(dy));
            // cordinate increments
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
                if (mat != nullptr && (x<mat->cols && y < mat->rows)) {
                    try {
                        mat->at<cv::Vec3b>(p.y, p.x) = cv::Vec3b(p.color.b, p.color.g, p.color.r);
                    }
                    catch (std::exception& ex) {

                    }
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
                point.x = round(transformedPointMat.at<double>(0, 0));
                point.y = round(transformedPointMat.at<double>(1, 0));

                // Translate the point back to its original position
                point.x += center.x;
                point.y += center.y;
            }
            updatePolygonCenter();
        }
        void _drawFrame(cv::Mat* mat) {
            m_frame_points = m_vertices;
            int n = m_vertices.size();

            for (int i = 0; i < n; i++) {
                int j = (i + 1) % n;
                _drawLineDDA(mat, nullptr, m_vertices[i], m_vertices[j]);
            }
            m_framed = true;
        }
    public:
        Polygon(const std::vector<Point>& _points) : m_vertices(_points), m_framed(false), m_frame_points(_points) {
            updatePolygonCenter();
        }
        Polygon(const std::vector<Point>& _points, const cv::Mat& _texture) : m_vertices(_points), m_texture(_texture), m_framed(false), m_frame_points(_points) {
            updatePolygonCenter();
        }
        Point mCenter = { 0,0 };
        void Draw(cv::Mat* mat) {
            if (m_texture.cols<=0) {
                _drawFrame(mat);
                _scanline(m_frame_points, mat, nullptr);
                m_frame_points = m_vertices;
                m_framed = false;
            }
            else {
                DrawTexture(mat);
            }
        }
        void DrawTexture(cv::Mat* mat) {
            _drawFrame(mat);
            _scanline(m_frame_points, mat, &m_texture);
            m_frame_points = m_vertices;
            m_framed = false;
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
        void WindowMap(const cv::Mat& viewport, const Point& window_start, const Point& window_end, Point point) {
            double lv = viewport.cols;
            double av = viewport.rows;
            cv::Mat mapping = (cv::Mat_<double>(3, 3) << 
                lv/(window_end.x-window_start.x), 0, (-window_start.x*lv) / (window_end.x - window_start.x) - 1,
                0, av / (window_end.y - window_start.y), (-window_start.y * av) / (window_end.y - window_start.y) - 1,
                0, 0, 1);
            this->_transformPolygon(mapping, point);
            for (auto& point : m_vertices) {
                if (point.x < 0) {
                    point.x = 0;
                }
                if (point.y < 0) {
                    point.y = 0;
                }
            }
        }
        void setPoints(std::vector<Point> newPoints) {
            m_vertices = newPoints;
        }
        void setTexture(cv::Mat texture) {
            m_texture = texture;
            std::cout << "";
        }
        
        bool containsPoint(const Point& point) const
        {
            int intersectCount = 0;
            const std::vector<Point>& points = this->getPoints();
            int numPoints = points.size();

            for (int i = 0; i < numPoints; ++i)
            {
                Point p1 = points[i];
                Point p2 = points[(i + 1) % numPoints];

                // Check if the point is on the same horizontal line as the current edge
                if ((p1.y <= point.y && point.y < p2.y) || (p2.y <= point.y && point.y < p1.y))
                {
                    // Check if the point is to the left of the current edge
                    if (point.x < (p2.x - p1.x) * (point.y - p1.y) / (p2.y - p1.y) + p1.x)
                    {
                        intersectCount++;
                    }
                }
            }
            // If the number of intersections is odd, the point is inside the polygon
            return (intersectCount % 2 == 1);
        }

        void updatePolygonCenter() {
            float totalX = 0.0;
            float totalY = 0.0;

            for (const Point& point : m_vertices) {
                totalX += point.x;
                totalY += point.y;
            }

            float centerX = totalX / m_vertices.size();
            float centerY = totalY / m_vertices.size();

            mCenter =  { centerX, centerY };
        }

        Color mapPointInTexture(double x, double y, const cv::Mat* tex) {
            if (x >= 1.0) {
                x = 1.0;
            }
            if (y >= 1.0) {
                y = 1.0;
            }
            if (x <= 0.0) {
                x = 0.0;
            }
            if (y <= 0.0) {
                y = 0.0;
            }
            int texX = round(x*(tex->cols - 1));
            int texY = round(y * (tex->rows - 1));
            cv::Vec3b mappedColor;
            try {
                mappedColor = tex->at<cv::Vec3b>(texY, texX);
            }
            catch (std::exception& ex) {
                std::cout << "Exception caught: " << ex.what() << std::endl;
            }
            return Color((int)mappedColor(0), (int)mappedColor(1), (int)mappedColor(2));
        }
        std::vector<Point> getPoints() const {
            return m_vertices;
        }
        cv::Mat getTexture() const {
            return m_texture;
        }
        void addPoint(const Point& point) {
            m_vertices.push_back(point);
        }
    };
}
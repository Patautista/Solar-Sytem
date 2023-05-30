namespace lib {
    Polygon* createRectangle(const Point& topLeft, const Point& bottomRight) {
        Point topRight(bottomRight.x, topLeft.y, topLeft.color);
        Point bottomLeft(topLeft.x, bottomRight.y, bottomRight.color);
        return new Polygon({ topLeft, topRight, bottomRight, bottomLeft });
    }

    Polygon* createOctagon(const Point& center, double size, const std::vector<Color>& colors) {
        Polygon* octagon = new Polygon({});

        // Calculate the angle between each pair of vertices
        double angleIncrement = 2 * 3.1415 / 8;
        double currentAngle = 0.0;

        // Iterate over each vertex position
        for (int i = 0; i < 8; ++i) {
            // Calculate the coordinates of the vertex based on the center and size
            double x = center.x + size * std::cos(currentAngle);
            double y = center.y + size * std::sin(currentAngle);

            // Create the point with or without color based on the provided vector
            if (i < colors.size()) {
                octagon->addPoint(Point(x, y, colors[i]));
            }
            else {
                octagon->addPoint(Point(x, y));
            }

            // Update the angle for the next vertex
            currentAngle += angleIncrement;
        }

        return octagon;
    }


    Polygon* createSquare(float size, Point position, std::vector<Color> colors) {
        if (colors.size()<4) {
            std::vector<Point> points = {
            Point(position.x - size / 2, position.y - size / 2, Color::white),
            Point(position.x - size / 2, position.y + size / 2, Color::white),
            Point(position.x + size / 2, position.y + size / 2, Color::white),
            Point(position.x + size / 2, position.y - size / 2, Color::white)
            };
            return new Polygon(points);
        }
        std::vector<Point> points = {
        Point(position.x - size / 2, position.y - size / 2, colors[0]),
        Point(position.x - size / 2, position.y + size / 2, colors[1]),
        Point(position.x + size / 2, position.y + size / 2, colors[2]),
        Point(position.x + size / 2, position.y - size / 2, colors[3])
        };
        return new Polygon(points);
    }
    Polygon* createCircle(float radius, Point center, int num_segments, Color color) {
        const double angle_step = 2 * CV_PI / num_segments;
        std::vector<Point> points;

        for (int i = 0; i < num_segments; ++i) {
            double angle = i * angle_step;
            int x = std::round(center.x + radius * std::cos(angle));
            int y = std::round(center.y + radius * std::sin(angle));
            points.emplace_back(x, y, color);
        }

        return new Polygon(points);
    }
    Polygon* createTriangle(int size, Point position, std::vector<Color> colors) {
        Polygon* triangle = new Polygon({});
        double scaleFactor = size / 100.0;
        // Define the three points of the triangle randomly
        srand(time(0));
        int x1 = position.x + rand() % (int)(50 * scaleFactor);
        int y1 = position.y + rand() % (int)(50 * scaleFactor);
        int x2 = position.x - rand() % (int)(50 * scaleFactor);
        int y2 = position.y + rand() % (int)(50 * scaleFactor);
        int x3 = position.x + rand() % (int)(50 * scaleFactor);
        int y3 = position.y - rand() % (int)(50 * scaleFactor);

        if (colors.size() < 3) {
            triangle->setPoints(std::vector<Point>({ Point(x1, y1), Point(x2, y2), Point(x3, y3) }));
        }
        else
        {
            triangle->setPoints(std::vector<Point>({ Point(x1, y1, colors[0]), Point(x2, y2, colors[1]), Point(x3, y3, colors[2])}));
        }
        return triangle;
    }
    void addPolygons(Polygon* poly1, Polygon* poly2) {
        std::vector<Point> points1 = poly1->getPoints();
        std::vector<Point> points2 = poly2->getPoints();

        points1.insert(points1.end(), points2.begin(), points2.end());

        poly1->setPoints(points1);
    }

}
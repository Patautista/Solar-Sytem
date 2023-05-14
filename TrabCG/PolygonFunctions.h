namespace lib {
    Polygon* createSquare(int size, Point position) {
        std::vector<Point> points = {
            Point(position.x - size / 2, position.y - size / 2),
            Point(position.x - size / 2, position.y + size / 2),
            Point(position.x + size / 2, position.y + size / 2),
            Point(position.x + size / 2, position.y - size / 2)
        };
        return new Polygon(points);
    }
    Polygon* createCircle(int radius, Point center) {
        const int num_segments = 360;
        const double angle_step = 2 * CV_PI / num_segments;
        std::vector<Point> points;

        for (int i = 0; i < num_segments; ++i) {
            double angle = i * angle_step;
            int x = std::round(center.x + radius * std::cos(angle));
            int y = std::round(center.y + radius * std::sin(angle));
            points.emplace_back(x, y);
        }

        return new Polygon(points);
    }
    void addPolygons(Polygon* poly1, Polygon* poly2) {
        std::vector<Point> points1 = poly1->getPoints();
        std::vector<Point> points2 = poly2->getPoints();

        points1.insert(points1.end(), points2.begin(), points2.end());

        poly1->setPoints(points1);
    }
}
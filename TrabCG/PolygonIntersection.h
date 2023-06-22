namespace lib {
    bool isPointOnSegment(const Point& p, const Point& q, const Point& r) {
        if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
            q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y)) {
            return true;
        }
        return false;
    }

    Point computeIntersection(const Point& p1, const Point& p2, const Point& p3, const Point& p4) {
        double x1 = p1.x;
        double y1 = p1.y;
        double x2 = p2.x;
        double y2 = p2.y;
        double x3 = p3.x;
        double y3 = p3.y;
        double x4 = p4.x;
        double y4 = p4.y;

        double denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

        if (denominator == 0.0) {
            // The line segments are parallel or coincident
            return Point(0, 0); // Invalid
        }

        double px = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / denominator;
        double py = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / denominator;

        return Point(px, py, p2.color);
    }

    int calculateOrientation(const Point& p, const Point& q, const Point& r) {
        int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

        if (val == 0) {
            return 0;  // Collinear points
        }
        return (val > 0) ? 1 : 2;  // Clockwise or counterclockwise
    }

    bool doSegmentsIntersect(const Point& p1, const Point& p2, const Point& q1, const Point& q2) {
        int o1 = calculateOrientation(p1, p2, q1);
        int o2 = calculateOrientation(p1, p2, q2);
        int o3 = calculateOrientation(q1, q2, p1);
        int o4 = calculateOrientation(q1, q2, p2);

        if (o1 != o2 && o3 != o4) {
            return true;
        }

        if (o1 == 0 && isPointOnSegment(p1, p2, q1)) {
            return true;
        }
        if (o2 == 0 && isPointOnSegment(p1, p2, q2)) {
            return true;
        }
        if (o3 == 0 && isPointOnSegment(q1, q2, p1)) {
            return true;
        }
        if (o4 == 0 && isPointOnSegment(q1, q2, p2)) {
            return true;
        }

        return false;
    }

    // Projection

    double squaredDistancePointToSegment(const Point& point, const Point& segmentStart, const Point& segmentEnd) {
        double dx = segmentEnd.x - segmentStart.x;
        double dy = segmentEnd.y - segmentStart.y;

        if (dx == 0 && dy == 0) {
            // The segment is a single point
            double px = point.x - segmentStart.x;
            double py = point.y - segmentStart.y;
            return px * px + py * py;
        }

        double t = ((point.x - segmentStart.x) * dx + (point.y - segmentStart.y) * dy) / (dx * dx + dy * dy);
        t = std::max(0.0, std::min(1.0, t));

        double projX = segmentStart.x + t * dx;
        double projY = segmentStart.y + t * dy;

        double px = point.x - projX;
        double py = point.y - projY;

        return px * px + py * py;
    }

    Point getClosestPointOnSegment(const Point& point, const Point& segmentStart, const Point& segmentEnd) {
        double dx = segmentEnd.x - segmentStart.x;
        double dy = segmentEnd.y - segmentStart.y;

        if (dx == 0 && dy == 0) {
            // The segment is a single point
            return segmentStart;
        }

        double t = ((point.x - segmentStart.x) * dx + (point.y - segmentStart.y) * dy) / (dx * dx + dy * dy);
        t = std::max(0.0, std::min(1.0, t));

        double projX = segmentStart.x + t * dx;
        double projY = segmentStart.y + t * dy;

        return Point(projX, projY);
    }

    Point projectPointOntoPolygon(const Point& point, const std::vector<Point>& polygonPoints) {
        double minDistance = std::numeric_limits<double>::max();
        Point closestPoint;

        // Iterate over each edge of the polygon
        for (size_t i = 0; i < polygonPoints.size(); i++) {
            const Point& p1 = polygonPoints[i];
            const Point& p2 = polygonPoints[(i + 1) % polygonPoints.size()];

            // Calculate the squared distance from the point to the line segment
            double distance = squaredDistancePointToSegment(point, p1, p2);

            // Update the closest point if the distance is smaller
            if (distance < minDistance) {
                minDistance = distance;
                closestPoint = getClosestPointOnSegment(point, p1, p2);
                closestPoint.color = point.color;
            }
        }

        return closestPoint;
    }

    // Intersection

    Polygon* findIntersection(const Polygon* polygon, const Polygon* window) {
        Polygon* intersection = new Polygon({});
        intersection->setTexture(polygon->getTexture());
        const std::vector<Point>& points1 = polygon->getPoints();
        const std::vector<Point>& points2 = window->getPoints();

        // Iterate over each point of the polygon
        for (size_t i = 0; i < points1.size(); i++) {
            const Point& p1 = points1[i];
            const Point& p2 = points1[(i + 1) % points1.size()];

            // Check if the point is inside the window
            if (window->containsPoint(p1)) {
                intersection->addPoint(p1);
            }
            if (!window->containsPoint(p1)) {
                Point projection = projectPointOntoPolygon(p1, points2);
                intersection->addPoint(projection);
            }

        }

        // Check if there are any intersection points
        if (intersection->getPoints().size() == 0) {
            delete intersection;
            return nullptr;
        }

        return intersection;
    }

}
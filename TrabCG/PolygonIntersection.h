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
            return Point(0, 0);  // Return an invalid point
        }

        double px = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / denominator;
        double py = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / denominator;

        return Point(px, py);
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
    Polygon* findIntersection(const Polygon* polygon1, const Polygon* polygon2) {
        Polygon* intersection = new Polygon({});
        auto points1 = polygon1->getPoints();
        auto points2 = polygon2->getPoints();

        // Iterate over each point of polygon1
        for (size_t i = 0; i < points1.size(); i++) {
            const Point& p1 = points1[i];
            const Point& p2 = points1[(i + 1) % points1.size()];
            if (polygon2->containsPoint(p1)) {
                intersection->addPoint(p1);
            }

            // Iterate over each point of polygon2
            for (size_t j = 0; j < points2.size(); j++) {
                const Point& q1 = points2[j];
                const Point& q2 = points2[(j + 1) % points2.size()];

                if (doSegmentsIntersect(p1, p2, q1, q2)) {
                    // Calculate the intersection point
                    Point intersectionPoint = computeIntersection(p1, p2, q1, q2);

                    // Add the intersection point to the new polygon
                    intersection->addPoint(intersectionPoint);
                }
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
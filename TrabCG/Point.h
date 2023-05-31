#include "Color.h"
namespace lib {
    class Point {
    public:
        double x, y;
        Color color;

        Point() : x(0), y(0), color(Color()) {}
        Point(double x_, double y_, Color color_) : x(x_), y(y_), color(color_) {}
        Point(double x, double y) : x(x), y(y), color(255, 255, 255) {}
    };
}
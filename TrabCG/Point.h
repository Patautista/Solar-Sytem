#include "Color.h"
namespace lib {
    class Point {
    public:
        int x, y;
        Color color;

        Point() : x(0), y(0), color(Color()) {}
        Point(float x_, float y_, Color color_) : x(x_), y(y_), color(color_) {}
        Point(float x, float y) : x(x), y(y), color(255, 255, 255) {}
    };
}
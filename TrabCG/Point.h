#include "Color.h"
namespace lib {
    class Point {
    public:
        int x, y;
        Color color;

        Point() : x(0), y(0), color(Color()) {}
        Point(int x_, int y_, Color color_) : x(x_), y(y_), color(color_) {}
        Point(int x, int y) : x(x), y(y), color(255, 255, 255) {}
    };
}
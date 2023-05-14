namespace lib {
    class Color {
    public:
        int r, g, b;

        Color() : r(0), g(0), b(0) {}
        Color(int red, int green, int blue) : r(red), g(green), b(blue) {}
    };

    class Point {
    public:
        int x, y;
        Color color;

        Point() : x(0), y(0), color(Color()) {}
        Point(int x_, int y_, Color color_) : x(x_), y(y_), color(color_) {}
        Point(int x, int y) : x(x), y(y), color(255, 255, 255) {}
    };
}
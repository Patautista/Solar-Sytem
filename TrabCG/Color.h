namespace lib {
    class Color {
    public:
        int r, g, b;

        static const Color red;
        static const Color blue;
        static const Color green;
        static const Color black;
        static const Color white;
        static const Color gray;
        static const Color cyan;
        static const Color purple;
        static const Color orange;
        static const Color yellow;
        static const Color magenta;
        static const Color salmon;
        static const Color brown;
        static const Color darkRed;
        static const Color darkBlue;
        static const Color darkGreen;
        static const Color darkCyan;
        static const Color darkYellow;
        static const Color cream;
        static const Color pastelRed;
        static const Color pastelGreen;
        static const Color pastelBlue;

        Color() : r(0), g(0), b(0) {}
        Color(int red, int green, int blue) : r(red), g(green), b(blue) {}
    };

    const Color Color::red = Color(255, 0, 0);
    const Color Color::blue = Color(0, 0, 255);
    const Color Color::green = Color(0, 255, 0);
    const Color Color::black = Color(0, 0, 0);
    const Color Color::white = Color(255, 255, 255);
    const Color Color::gray = Color(128, 128, 128);
    const Color Color::cyan = Color(0, 255, 255);
    const Color Color::purple = Color(128, 0, 128);
    const Color Color::orange = Color(255, 165, 0);
    const Color Color::yellow = Color(255, 255, 0);
    const Color Color::magenta = Color(255, 0, 255);
    const Color Color::salmon = Color(250, 128, 114);
    const Color Color::brown = Color(139, 69, 19);
    const Color Color::darkRed = Color(150, 0, 0);
    const Color Color::darkBlue = Color(0, 0, 139);
    const Color Color::darkGreen = Color(0, 100, 0);
    const Color Color::darkCyan = Color(0, 139, 139);
    const Color Color::darkYellow = Color(139, 139, 0);
    const Color Color::cream = Color(255, 253, 208);
    const Color Color::pastelRed = Color(255, 204, 203);
    const Color Color::pastelGreen = Color(204, 255, 204);
    const Color Color::pastelBlue = Color(204, 230, 255);
}

#include "Point.h"
#include "Polygon.h"
#include "PolygonFunctions.h"
namespace lib {
	class Window {
	public:
		Window(const Point start, Point end) : start(start), end(end) {
			window = createRectangle(start, end);
		}
		Polygon* window;
		Point start;
		Point end;
	};
}
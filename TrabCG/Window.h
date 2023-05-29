#include "Point.h"
#include "Polygon.h"
#include "PolygonFunctions.h"
namespace lib {
	class Window {
	private:
		Polygon m_window = Polygon({});
	public:
		Window(const Point& start, Point& end) : start(start), end(end){
			auto temp = createRectangle(start, end);
			m_window = *temp;
			delete(temp);
		}
		Point start;
		Point end;
		void Translate(float x, float y) {
			m_window.Translate(x, y, Point(0,0));
		}
	};
}
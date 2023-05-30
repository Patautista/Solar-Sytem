#include <vector>
namespace lib {
	class Window {
	private:
		Polygon m_window = Polygon({});
		std::vector<Polygon> m_intersections;
		std::vector<Polygon*> m_visible_polygons;
		void UpdateIntersections() {
			m_intersections = {};
			for (auto& poly : m_visible_polygons) {
				auto temp = findIntersection(poly, &m_window);
				if (temp != nullptr) {
					m_intersections.push_back(*temp);
				}
				delete(temp);
			}
		}
	public:
		Window(const Point& start, Point& end) : start(start), end(end){
			auto temp = createRectangle(start, end);
			m_window = *temp;
			center = Point((end.x + start.x)/2, (end.y - start.y)/2);
			delete(temp);
		}
		Point start;
		Point end;
		Point center;
		void Translate(float x, float y) {
			m_window.Translate(x, y, Point(0,0));
			start = m_window.getPoints()[0];
			end = m_window.getPoints()[2];
		}
		void Zoom(float factor) {
			m_window.Scale(factor, factor, Point(0, 0));
			start = m_window.getPoints()[0];
			end = m_window.getPoints()[2];
		}
		void UpdateVisiblePolygons(std::vector<Polygon*> polygons) {
			m_visible_polygons = polygons;
		}
		void Draw(cv::Mat* viewport) {
			UpdateIntersections();
			for (auto& poly : m_intersections) {
				auto temp = poly;
				temp.WindowMap(*viewport, this->start, this->end, Point(0, 0));
				temp.Draw(viewport);
			}
		}
	};
}
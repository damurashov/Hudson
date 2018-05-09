#include <QApplication>
#include "osm_widget.h"

using namespace ns_osm;

int main(int argc, char* argv[]) {
//	Osm_Map::GEO_DEGREE_MULTIPLIER = 1000.0;
	QApplication a(argc, argv);
	Osm_Widget widget;
	Osm_Message message;
	Osm_Map& map = *(widget.mp_map);

//	map.add(new Osm_Node(0,0));
//	map.add(new Osm_Node(5,5));
//	map.add(new Osm_Node(15,15));

//	qDebug() << message(widget.load_from_xml(PATH_MERKAARTOR_MAP));
	qDebug() << message(widget.load_from_xml(PATH_GENUINE_MAP));
//	qDebug() << message(widget.load_from_xml(PATH_TEST_MAP));

	int counter = 0;
	for (auto it = map.nbegin(); it != map.nend(); ++it) {
		counter++;
	}
	qDebug() << "NODES: " << counter;

	counter = 0;
	for (auto it = map.wbegin(); it != map.wend(); ++it) {
		counter++;
	}
	qDebug() << "WAYS: " << counter;

	qDebug() << "LEFT: " << map.get_scene_rect().left();
	qDebug() << "TOP: " << map.get_scene_rect().top();
	qDebug() << "RIGHT: " << map.get_scene_rect().right();
	qDebug() << "BOTTOM: " << map.get_scene_rect().bottom();

	QRectF rect_from_map(map.get_scene_rect());
	QRectF rect_from_scene(widget.mp_view_handler->mp_scene->sceneRect());

	qDebug() << "Scene rect map: \t\t Scene rect scene";
	qDebug() << "left: " << rect_from_map.left() << "\t\t" << rect_from_scene.left();
	qDebug() << "top: " << rect_from_map.top() << "\t\t" << rect_from_scene.top();
	qDebug() << "right: " << rect_from_map.right() << "\t\t" << rect_from_scene.right();
	qDebug() << "bottom: " << rect_from_map.bottom() << "\t\t" << rect_from_scene.bottom();

	for (auto it = map.nbegin(); it != map.nend(); ++it) {
		Osm_Node* p_node = it.value();
		QPointF pos(widget.mp_view_handler->m_nodeid_to_item[p_node->get_id()]->pos());
		qDebug() << "Node. Lat: " << p_node->get_lat() << ". Lon: " << p_node->get_lon();
		qDebug() << "Node on map. Pos Y: " << pos.y() << ". Pos X: " << pos.x();
		qDebug() << "---------------------------------------------";
	}
	qDebug() << widget.mp_view_handler->mp_view->transform().m11();
	qDebug() << widget.mp_view_handler->mp_view->transform().m22();

	widget.show();
	return a.exec();
}

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

	widget.show();
	return a.exec();
}

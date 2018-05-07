#include <QApplication>
#include "osm_widget.h"

using namespace ns_osm;

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
	Osm_Widget widget;
	Osm_Message message;
	Osm_Map& map = *(widget.mp_map);

	map.add(new Osm_Node(0,0));
	map.add(new Osm_Node(5,5));
	map.add(new Osm_Node(15,15));

	//qDebug() << message(widget.load_from_xml(PATH_TEST_MAP));
	widget.show();

	return a.exec();
}

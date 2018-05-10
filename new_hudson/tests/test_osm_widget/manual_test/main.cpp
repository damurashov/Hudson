#include <QApplication>
#include "osm_widget.h"

using namespace ns_osm;

int main(int argc, char* argv[]) {
//	Osm_Map::GEO_DEGREE_MULTIPLIER = 1000.0;
	QApplication a(argc, argv);
	Osm_Widget widget;

	widget.show();

//	widget.load_from_xml(PATH_MERKAARTOR_MAP);
	widget.load_from_xml(PATH_GENUINE_MAP);
	widget.save_to_xml(PATH_MAP2);

	return a.exec();
}

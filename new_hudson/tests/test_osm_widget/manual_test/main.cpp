#include <QApplication>
#include "osm_widget.h"
#include "hudson_app.h"

using namespace ns_osm;

int main(int argc, char* argv[]) {
//	Osm_Map::GEO_DEGREE_MULTIPLIER = 1000.0;
	QApplication a(argc, argv);
	Hudson_App app;
	app.mp_osm_widget->load_from_xml(PATH_GENUINE_MAP);
	app.show();

	return a.exec();
}

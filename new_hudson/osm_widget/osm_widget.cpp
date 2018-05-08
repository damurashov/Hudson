#include "osm_widget.h"
using namespace ns_osm;

/*================================================================*/
/*                  Constructors, destructors                     */
/*================================================================*/

Osm_Widget::Osm_Widget(QWidget* p_parent) :
                       QWidget(p_parent)
{
	mp_map = new Osm_Map;
//	mp_map->set_force_use_dynamic_bound(true);
	mp_map->adopt();
	mp_xml_handler = new Xml_Handler(*mp_map);
	mp_view_handler = new View_Handler(*mp_map);
	setLayout(new QHBoxLayout);
	layout()->addWidget(mp_view_handler);
}

Osm_Widget::~Osm_Widget() {
	mp_map->orphan();
	delete mp_view_handler;
	delete mp_xml_handler;
	if (mp_map->count_parents() == 0) {
		mp_map->clear();
		delete mp_map;
	}
}

/*================================================================*/
/*                        Public methods                          */
/*================================================================*/

int Osm_Widget::load_from_xml(const QString &xml_path) {
//	return m_xml_handler.load_from_xml(xml_path);
	return mp_xml_handler->load_from_xml(xml_path);
}

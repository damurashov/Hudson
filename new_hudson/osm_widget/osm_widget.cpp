#include "osm_widget.h"
using namespace ns_osm;
// ===================== Constructors, destructors =====================

Osm_Widget::Osm_Widget(QWidget* p_parent) :
                       QWidget(p_parent),
                       mp_map(new Osm_Map),
                       m_xml_handler(*mp_map)
{
	mp_map->adopt();
}

Osm_Widget::Osm_Widget(const Osm_Widget& wgt, QWidget *p_parent) :
                       QWidget(p_parent),
                       mp_map(wgt.mp_map),
                       m_xml_handler(*mp_map)
{
	mp_map->adopt();
}

Osm_Widget::~Osm_Widget() {
	mp_map->orphan();
	if (mp_map->count_parents() == 0) {
		mp_map->clear();
	}
}

// ========================== Public methods =========================

int Osm_Widget::load_from_xml(const QString &xml_path) {
	int code;

	return m_xml_handler.load_from_xml(xml_path);
}

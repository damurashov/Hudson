#include "osm_widget.h"
using namespace ns_osm;
// ===================== Constructors, destructors =====================

Osm_Widget::Osm_Widget(QWidget* p_parent) :
    QWidget(p_parent),
    m_xml_handler(m_map) {}

Osm_Widget::~Osm_Widget() {
	remove_all_osm_objects();
}

// ========================== Public methods =========================

int Osm_Widget::load_from_xml(const QString &xml_path) {
	int code;

	return m_xml_handler.load_from_xml(xml_path);
}

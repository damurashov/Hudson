#ifndef OSM_WIDGET_H
#define OSM_WIDGET_H

#include "xml_handler.h"
#include "osm_elements.h"
#include "osm_message.h"

class Osm_Widget : public QWidget {
private:
	Osm_Map				m_map;
	ns_osm::Xml_Handler	m_xml_handler;
public:
	Osm_Map&			get_map					();
	int					load_from_xml			(const QString& xml_path);
	                    Osm_Widget				(QWidget* p_parent = nullptr);
	virtual				~Osm_Widget				();
};

#endif // OSM_WIDGET_H

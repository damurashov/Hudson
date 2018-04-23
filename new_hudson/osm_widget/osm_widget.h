#ifndef OSM_WIDGET_H
#define OSM_WIDGET_H

#include "xml_handler.h"
#include "osm_elements.h"
#include "osm_message.h"
#include "osm_map.h"

class Osm_Widget : public QWidget {
private:
	ns_osm::Osm_Map*	mp_map;
	ns_osm::Xml_Handler	m_xml_handler;
public:
	ns_osm::Osm_Map&	get_map					();
	int					load_from_xml			(const QString& xml_path);
	Osm_Widget&			operator=				(const Osm_Widget&) = delete;
	                    Osm_Widget				(QWidget* p_parent = nullptr);
						Osm_Widget				(const Osm_Widget&, QWidget* p_parent = nullptr);
	virtual				~Osm_Widget				();
};

#endif // OSM_WIDGET_H

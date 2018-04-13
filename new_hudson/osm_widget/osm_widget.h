#ifndef OSM_WIDGET_H
#define OSM_WIDGET_H

#include "xml_handler.h"
#include "osm_elements.h"

class Osm_Widget : public QWidget {
protected:
	struct Osm_Xml;
private:
	QHash<long long, ns_osm::Osm_Node*>		m_nodes_hash;
	QHash<long long, ns_osm::Osm_Way*>		m_ways_hash;
	QHash<long long, ns_osm::Osm_Relation*>	m_relations_hash;
	QRectF									m_bounding_rect;
	ns_osm::Xml_Handler						m_xml_handler;
protected:
	void									remove_all_osm_objects	();
	void									compute_bounding_rect	();
public:
	bool									load_from_xml			(const QString& xml_path);
	                                        Osm_Widget				(QWidget* p_parent = nullptr);
	virtual									~Osm_Widget				();
};

#endif // OSM_WIDGET_H

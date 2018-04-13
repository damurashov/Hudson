#ifndef XML_HANDLER_H
#define XML_HANDLER_H

#include "osm_elements.h"

#ifndef QT_WIDGETS_H
#define QT_WIDGETS_H
#include <QtWidgets>
#endif // Include Guard QT_WIDGETS_H

#ifndef QT_XML_H
#define QT_XML_H
#include <QtXml>
#endif // Include Guard QT_XML_H

namespace ns_osm  {

class Xml_Handler {
private:
	                                    Xml_Handler();
protected:
	struct Osm_Xml;

	QHash<long long, Osm_Node*>&		m_nodes_hash;
	QHash<long long, Osm_Way*>&			m_ways_hash;
	QHash<long long, Osm_Relation*>&	m_relations_hash;
	QRectF&								m_bounding_rect;

	void								load_bound_from_xml		(const QDomNode& node);
	void								load_nodes_from_xml		(const QDomNode& node);
	void								load_ways_from_xml		(const QDomNode& node);
	void								load_relations_from_xml	(const QDomNode& node);
public:
	bool								load_from_xml			(const QString& xml_path);
	                                    Xml_Handler				(QHash<long long, Osm_Node*>&,
										                         QHash<long long, Osm_Way*>&,
										                         QHash<long long, Osm_Relation*>&,
										                         QRectF&);
}; /* class Xml_Handler */
}/* namespace  */

#include "osm_xml.h"
#endif // XML_HANDLER_H

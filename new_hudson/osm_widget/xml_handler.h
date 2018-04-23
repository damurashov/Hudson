#ifndef XML_HANDLER_H
#define XML_HANDLER_H

#include "osm_elements.h"
#include "osm_message.h"

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
	                        Xml_Handler				()						= delete;
							Xml_Handler				(const Xml_Handler&)	= delete;
	Xml_Handler&			operator=				(const Xml_Handler&)	= delete;
protected:
	struct Osm_Xml;

	ns_osm::Osm_Map&		m_map;

	void					load_bound_from_xml		(const QDomNode& node);
	void					load_nodes_from_xml		(const QDomNode& node);
	void					load_ways_from_xml		(const QDomNode& node);
	void					load_relations_from_xml	(const QDomNode& node);
public:
	int						load_from_xml			(const QString& xml_path);
	                        Xml_Handler				(Osm_Map&);
}; /* class Xml_Handler */
}/* namespace  */

#include "osm_xml.h"
#endif // XML_HANDLER_H

#ifndef OSM_WIDGET_H
#define OSM_WIDGET_H

#include "xml_handler/xml_handler.h"
#include "view_handler/view_handler.h"
#include "info_widget/info_widget.h"
#include "osm_elements.h"
#include "osm_message.h"

class Osm_Widget : public QWidget {
private:
	ns_osm::Info_Widget*	mp_info_widget;
	ns_osm::Osm_Map*		mp_map;
	ns_osm::Xml_Handler*	mp_xml_handler;
	ns_osm::View_Handler*	mp_view_handler;
public:
	int						save_to_xml				(const QString& xml_path);
	int						load_from_xml			(const QString& xml_path);
	                        Osm_Widget				(QWidget* p_parent = nullptr);
	Osm_Widget&				operator=				(const Osm_Widget&) = delete;
	                        Osm_Widget				(const Osm_Widget&) = delete;
	virtual					~Osm_Widget				();
};

#endif // OSM_WIDGET_H

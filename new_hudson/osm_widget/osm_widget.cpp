#include "osm_widget.h"
using namespace ns_osm;

/*================================================================*/
/*                  Constructors, destructors                     */
/*================================================================*/

Osm_Widget::Osm_Widget(QWidget* p_parent) :
                       QWidget(p_parent)
{
	QSplitter* p_splitter = new QSplitter(Qt::Horizontal, this);
//	setMinimumHeight(400);
//	setMinimumWidth(600);
	mp_map = new Osm_Map;
	mp_map->set_force_use_dynamic_bound(true);
	mp_map->adopt();
	mp_xml_handler = new Xml_Handler(*mp_map);
	mp_view_handler = new View_Handler(*mp_map);
	mp_info_widget = new Info_Widget(this);
//	mp_info_widget->setMinimumWidth(200);
	setLayout(new QHBoxLayout(this));
	p_splitter->addWidget(mp_info_widget);
	p_splitter->addWidget(mp_view_handler);
	p_splitter->setStretchFactor(0, 1);
	p_splitter->setStretchFactor(1, 5);
	layout()->addWidget(p_splitter);
	/*layout()->addWidget(mp_info_widget);
	layout()->addWidget(mp_view_handler)*/;

	QObject::connect(mp_view_handler, SIGNAL(signal_object_selected(Osm_Node&)),
	                 mp_info_widget, SLOT(slot_object_selected(Osm_Node&)));
	QObject::connect(mp_view_handler, SIGNAL(signal_object_selected(Osm_Way&)),
	                 mp_info_widget, SLOT(slot_object_selected(Osm_Way&)));
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

int Osm_Widget::save_to_xml(const QString& xml_path) {
	return mp_xml_handler->save_to_xml(xml_path);
}

int Osm_Widget::load_from_xml(const QString &xml_path) {
//	return m_xml_handler.load_from_xml(xml_path);
	mp_map->clear();
	return mp_xml_handler->load_from_xml(xml_path);
}

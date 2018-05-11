#include "info_widget.h"

using namespace ns_osm;

/*================================================================*/
/*                  Constructors, destructors                     */
/*================================================================*/

Info_Widget::Info_Widget(QWidget* p_parent) : QWidget(p_parent) {
	QToolBar* p_toolbar = new QToolBar(this);
	mp_tag_table = new Tag_Table;

//	p_toolbar->addAction("Update", mp_tag_table, SLOT(slot_update()));
	p_toolbar->addAction("Push row", mp_tag_table, SLOT(slot_push_row()));
	p_toolbar->addAction("Pop row", mp_tag_table, SLOT(slot_pop_row()));
	p_toolbar->addAction("Delete selected", mp_tag_table, SLOT(slot_delete_selected()));
	p_toolbar->addAction("Clear", mp_tag_table, SLOT(slot_delete_all()));
	p_toolbar->setMaximumHeight(50);
	setMaximumWidth(400);

	setLayout(new QVBoxLayout(this));
	layout()->addWidget(p_toolbar);
	layout()->addWidget(mp_tag_table);
}

/*================================================================*/
/*                      Protected methods                         */
/*================================================================*/

void Info_Widget::handle_event_delete(Osm_Node&) {
	mp_tag_table->unset_info();
}

void Info_Widget::handle_event_delete(Osm_Way&) {
	mp_tag_table->unset_info();
}

/*================================================================*/
/*                        Public methods                          */
/*================================================================*/

void Info_Widget::slot_object_selected(Osm_Node& node) {
	unsubscribe();
	subscribe(node);
	mp_tag_table->set_info(node);
}

void Info_Widget::slot_object_selected(Osm_Way& way) {
	unsubscribe();
	subscribe(way);
	mp_tag_table->set_info(way);
}

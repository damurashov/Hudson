#include "view_handler.h"

using namespace ns_osm;

/*================================================================*/
/*                  Constructors, destructors                     */
/*================================================================*/

View_Handler::View_Handler(Osm_Map& map) : m_map(map) {
	load_from_map();
	subscribe(m_map);
}

View_Handler::View_Handler(const View_Handler& vhandler) : m_map(vhandler.m_map) {
//	f_has_info_table = vhandler.f_has_info_table;
	f_editable = vhandler.f_editable;
	load_from_map();
	subscribe(m_map);
}

View_Handler::~View_Handler() {
	while(!m_nodeid_to_item.isEmpty()) {
		remove(m_nodeid_to_item.begin().value()->get_node());
	}
	while (!m_wayid_to_item.isEmpty()) {
		remove(m_wayid_to_item.begin().value()->get_way());
	}
}

/*================================================================*/
/*                       Private methods                          */
/*================================================================*/

void View_Handler::slot_blank_area_clicked(QPointF, Qt::MouseButton) {
	/* Drawing operations will be implemented here soon */
}

void View_Handler::slot_node_clicked(Osm_Node *, Qt::MouseButton) {
	/* Drawing operations will be implemented here soon */
}

void View_Handler::slot_edge_clicked(QPointF point,
                                     Osm_Way* p_way,
                                     Osm_Node* p_node1,
                                     Osm_Node* p_node2,
                                     Qt::MouseButton button)
{
	/* Drawing operations will be implemented here soon */
}

void View_Handler::add(Osm_Node* p_node) {
	Item_Node* p_nodeitem;

	if (p_node == nullptr) {
		return;
	} else if (m_nodeid_to_item.contains(p_node->get_id())) {
		return;
	}
	subscribe(*p_node);
	p_nodeitem = new Item_Node(m_map, *p_node);
	mp_scene->addItem(p_nodeitem);
	m_nodeid_to_item.insert(p_node->get_id(), p_nodeitem);
	QObject::connect(p_nodeitem,
	                 SIGNAL(signal_node_clicked(Osm_Node*,Qt::MouseButton)),
	                 this,
	                 SLOT(slot_node_clicked(Osm_Node*,Qt::MouseButton)));
}

void View_Handler::add(Osm_Way* p_way) {
	Item_Way*	p_item_way;

	if (p_way == nullptr) {
		return;
	} else if (m_wayid_to_item.contains(p_way->get_id())) {
		return;
	}

	subscribe(*p_way);
	p_item_way = new Item_Way(m_map, *this, *p_way);
	mp_scene->addItem(p_item_way);
	m_wayid_to_item.insert(p_way->get_id(), p_item_way);
}

void View_Handler::remove(Osm_Node* p_node) {
	Item_Node* p_nodeitem;
	if (p_node == nullptr) {
		return;
	}
	if (!m_nodeid_to_item.contains(p_node->get_id())) {
		return;
	}
	p_nodeitem = m_nodeid_to_item[p_node->get_id()];
	mp_scene->removeItem(p_nodeitem);
	m_nodeid_to_item.remove(p_node->get_id());
	delete p_nodeitem;
}

void View_Handler::remove(Osm_Way* p_way) {
	Item_Way*	p_item_way;

	if (p_way == nullptr) {
		return;
	}
	if (!m_wayid_to_item.contains(p_way->get_id())) {
		return;
	}

	p_item_way = m_wayid_to_item[p_way->get_id()];
	mp_scene->removeItem(p_item_way);
	m_wayid_to_item.remove(p_way->get_id());

	delete p_item_way;
}

void View_Handler::load_from_map() {
	mp_scene = new QGraphicsScene(this);
	mp_view = new Osm_View;
	mp_scene->setSceneRect(m_map.get_scene_rect());
	mp_view->setScene(mp_scene);
	setLayout(new QHBoxLayout(this));
	layout()->addWidget(mp_view);
	QObject::connect(mp_view,
	                 SIGNAL(signal_blank_area_clicked(QPointF,Qt::MouseButton)),
	                 this,
	                 SLOT(slot_blank_area_clicked(QPointF,Qt::MouseButton)));
	for (Osm_Map::node_iterator it = m_map.nbegin(); it != m_map.nend(); ++it) {
		add(*it);
	}
	for (Osm_Map::way_iterator it = m_map.wbegin(); it != m_map.wend(); ++it) {
		add(*it);
	}
}

/*================================================================*/
/*                      Protected methods                         */
/*================================================================*/

void View_Handler::handle_event_delete(Osm_Node& node) {
	remove(&node);
}

void View_Handler::handle_event_delete(Osm_Way& way) {
	remove(&way);
}

void View_Handler::handle_event_update(Osm_Object&) {
	Meta meta(get_meta());
	switch (meta) {
	case MAP_EVENT:
		if (meta.get_subject() == nullptr) {
			return;
		}
		switch (meta.get_event()) {
		case MAP_NODE_ADDED:
			add(static_cast<Osm_Node*>(meta.get_subject()));
			break;
		case MAP_WAY_ADDED:
			add(static_cast<Osm_Way*>(meta.get_subject()));
			break;
		default:
			break;
		}
	default:
		break;
	}
}

void View_Handler::handle_event_delete(Osm_Object&) {
	switch (get_meta()) {
	case MAP_DELETED:
		while(!m_nodeid_to_item.isEmpty()) {
			remove(m_nodeid_to_item.begin().value()->get_node());
		}
		while (!m_wayid_to_item.isEmpty()) {
			remove(m_wayid_to_item.begin().value()->get_way());
		}
	default:
		break;
	}
}

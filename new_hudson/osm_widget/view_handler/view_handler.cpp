#include "view_handler.h"

using namespace ns_osm;

/*================================================================*/
/*                  Constructors, destructors                     */
/*================================================================*/

View_Handler::View_Handler(Osm_Map& map) : m_map(map) {
	load_from_map();
}

View_Handler::View_Handler(const View_Handler& vhandler) : m_map(vhandler.map) {
	f_has_info_table = vhandler.f_has_info_table;
	f_editable = vhandler.f_editable;
	load_from_map();
}

View_Handler::~View_Handler() {
	while(!m_nodeid_to_item.isEmpty()) {
		remove(m_nodeid_to_item.begin().key());
	}
	while (!m_wayid_to_item.isEmpty()) {
		remove(m_wayid_to_item.begin().key());
	}
	/* TODO: implement information table */
	/* TODO: check parentship for information table */
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
	}
	p_nodeitem = new Item_Node(m_map, *p_node);
	mp_scene->addItem(p_nodeitem);
	m_nodeid_to_item.insert(p_node->get_id(), p_nodeitem);
	QObject::connect(p_nodeitem,
	                 SIGNAL(signal_node_clicked(Osm_Node*,Qt::MouseButton)),
	                 this,
	                 SLOT(slot_node_clicked(Osm_Node*,Qt::MouseButton)));
}

void View_Handler::add(Item_Edge* p_item_edge) {
	if (p_item_edge == nullptr) {
		return;
	}
	mp_scene->addItem(p_item_edge);
	QObject::connect(p_item_edge,
	                 SIGNAL(signal_edge_clicked(QPointF,Osm_Way*,Osm_Node*,Osm_Node*,Qt::MouseButton)),
	                 this,
	                 SLOT(slot_edge_clicked(QPointF,Osm_Way*,Osm_Node*,Osm_Node*,Qt::MouseButton)));
}


void View_Handler::add(Osm_Way* p_way) {
	Item_Way*	p_item_way;
	QList<Edge>	edges;
	Item_Edge*	p_item_edge;

	if (p_way == nullptr) {
		return;
	}
	p_item_way = new Item_Way(m_map, *p_way);
	mp_scene->addItem(p_item_way);
	m_wayid_to_item.insert(p_way->get_id(), p_item_way);

	edges = Edge::to_edge_list(*p_way);
	for (auto it = edges.begin(); it != edges.end(); ++it) {
		p_item_edge = p_item_way->emplace_item(*it);
		add(p_item_edge);
	}
}

void View_Handler::remove(Osm_Node* p_node) {
	Item_Node* p_nodeitem;
	if (p_node == nullptr) {
		return;
	}
	if (!m_nodeid_to_item.contains(p_node->get_id())) {
		return;
	}
	p_nodeitem = m_nodeid_to_item[p_node];
	mp_scene->removeItem(p_nodeitem);
	m_nodeid_to_item.remove(p_node->get_id());
	delete p_nodeitem;
}

void View_Handler::remove(Item_Edge* p_item_edge) {
	if (p_item_edge == nullptr) {
		return;
	}

	mp_scene->removeItem(p_item_edge);
	QObject::disconnect(p_item_edge,
	                    SIGNAL(signal_edge_clicked(QPointF,Osm_Way*,Osm_Node*,Osm_Node*,Qt::MouseButton)),
	                    this,
	                    SLOT(slot_edge_clicked(QPointF,Osm_Way*,Osm_Node*,Osm_Node*,Qt::MouseButton)));
}

void View_Handler::remove(Osm_Way* p_way) {
	Item_Way*	p_item_way;
	Item_Edge*	p_item_edge;
	QList<Edge>	edges;

	if (p_way == nullptr) {
		return;
	}
	if (!m_nodeid_to_item.contains(p_way->get_id())) {
		return;
	}

	p_item_way = m_wayid_to_item[p_way];
	mp_scene->removeItem(p_item_way);
	m_wayid_to_item.remove(p_way->get_id());

	edges = Edge::to_edge_list(*p_way);
	for (auto it = edges.begin(); it != edges.end(); ++it) {
		p_item_edge = p_item_way->get_item(*it);
		remove(p_item_edge);
		p_item_way->remove(p_item_edge);
	}

	delete p_item_way;
}

void View_Handler::apply_layout() {
	layout()->removeWidget(mp_splitter);
	layout()->removeWidget(mp_view);
	layout()->addWidget(mp_view);
}

void View_Handler::load_from_map() {
	mp_scene = new QGraphicsScene(this);
	mp_view = new Osm_View();
	mp_view->setScene(mp_scene);
	mp_splitter = new QSplitter(this);
	setLayout(new QHBoxLayout(this));
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

void View_Handler::handle_event_update(Osm_Way& way) {
	QList<Edge>			edges;
	QList<Item_Edge*>	edge_items;
	Item_Edge*			p_item_edge;
	Item_Way*			p_item_way = m_wayid_to_item[way->get_id()];

	if (p_item_way == nullptr) {
		return;
	}

	switch (get_meta()) {
	case NODE_ADDED:
		edges = p_item_way->get_added();
		for (auto it = edges.begin(); it != edges.end(); ++it) {
			p_item_edge = p_item_way->emplace_item(*it);
			if (p_item_edge == nullptr) {
				return;
			}
			add(p_item_edge);
			break;
		}
	case NODE_DELETED:
		edges = p_item_way->get_deleted();
		for (auto it = edges.begin(); it != edges.end(); ++it) {
			p_item_edge = *it;
			remove(p_item_edge);
			p_item_way->remove_item(p_item_edge);
			break;
		}
	}

	p_item_way->update();
}

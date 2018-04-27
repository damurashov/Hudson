#include "item_way.h"

using namespace ns_osm;

/*================================================================*/
/*                  Constructors, destructors                     */
/*================================================================*/

Item_Way::Item_Way(const Osm_Map& map,
                   Osm_Way& way,
                   QObject *p_parent)
                   : QObject(p_parent),
                     m_map(map),
                     m_way(way)
{
	subscribe(m_way);
	compose_group();
}

/*================================================================*/
/*                      Protected methods                         */
/*================================================================*/

void Item_Way::handle_node_added() {
	enum Pos {FRONT, MID, BACK}				pos;
	QList<Item_Edge*>::iterator				it_edge = m_edges.begin();
	const QList<Osm_Node*>::const_iterator	it_node = m_way.get_nodes_list().cbegin();
	Item_Edge*								p_edge = *it_edge;
	Osm_Node*								p_node = const_cast<Osm_Node*>(*it_node);

	/* Seek for the position where the new node has been added */
	if (p_node != p_edge->first()) {
		pos = FRONT;
	} else {
		it_node++;
		while (it_node != m_way.get_nodes_list().cend()) {
			p_node = const_cast<Osm_Node*>(*it_node);
			p_edge = *it_edge;

			if (p_node != p_edge->second()) {
				pos = MID;
				break;
			}
			it_node++;
			if (it_edge == m_edges.end()) {
				pos = BACK;
				break;
			} else {
				it_edge++;
			}
		}
	}
	/* Emplace new edge(s) */
	switch (pos) {
	case FRONT:
		m_edges.push_front(new Item_Edge(m_map, m_way, *p_node, *(p_edge->first())));
		addToGroup(m_edges.front());
		scene()->addItem(m_edges.front());
		break;
	case BACK:
		m_edges.push_back(new Item_Edge(m_map, m_way, *(p_edge->second()), *p_node));
		addToGroup(m_edges.back());
		scene()->addItem(m_edges.back());
		break;
	case MID:
		/* Insert new edges... */
		it_edge = m_edges.insert(it_edge, new Item_Edge(m_map, m_way, *p_node, *(p_edge->second())));
		it_edge = m_edges.insert(it_edge, new Item_Edge(m_map, m_way, *(p_edge->first()), *p_node));
		addToGroup(*it_edge);
		scene()->addItem(*it_edge);
		it_edge++;
		addToGroup(*it_edge);
		scene()->addItem(*it_edge);
		it_edge++;
		/* ... and remove the old one */
		removeFromGroup(*it_edge);
		scene()->removeItem(*it_edge);
		m_edges.removeOne(*it_edge);
		delete *it_edge;
	}
}

void Item_Way::handle_node_deleted() {
	enum Pos {FRONT, MID, BACK}				pos;
	const QList<Osm_Node*>::const_iterator	it_node = m_way.get_nodes_list().cbegin();
	QList<Item_Edge*>::iterator				it_edge = m_edges.begin();
	Osm_Node*								p_node;
	Osm_Node*								p2_node;
	Item_Edge*								p_edge;
	Item_Edge*								p2_edge;

	/* Find pos */
	if (m_edges.front()->first() != const_cast<Osm_Node*>(m_way.get_nodes_list().front())) {
		pos = FRONT;
	} else if (m_edges.back()->second() != const_cast<Osm_Node*>(m_way.get_nodes_list().back())) {
		pos = BACK;
	} else {
		pos = MID;
		it_node++;
		while (it_edge != m_edges.end()) {
			p_node = const_cast<Osm_Node*>(*it_node);
			if (p_node != (*it_edge)->second() || it_node == m_way.get_nodes_list().cend()) {
				break;
			}
			it_node++;
			it_edge++;
		}
	}
	/* Remove edge */
	switch (pos) {
	case FRONT:
		scene()->removeItem(m_edges.front());
		removeFromGroup(m_edges.front());
		delete m_edges.front();
		m_edges.removeFirst();
		break;
	case BACK:
		scene()->removeItem(m_edges.back());
		removeFromGroup(m_edges.back());
		delete m_edges.back();
		m_edges.removeLast();
		break;
	case MID:
		p_node = (*it_edge)->first();
		p_edge = *it_edge;
		it_edge++;
		p2_node = (*it_edge)->second();
		p2_edge = *it_edge;
		it_edge--;
		it_edge = m_edges.insert(it_edge, new Item_Edge(m_map, m_way, *p_node, *p2_node));
		m_edges.removeOne(p_edge);
		m_edges.removeOne(p2_edge);
		scene()->removeItem(p_edge);
		scene()->removeItem(p2_edge);
		removeFromGroup(p_edge);
		removeFromGroup(p2_edge);
		delete p_edge;
		delete p2_edge;
		break;
	}
}

void Item_Way::compose_group() {
	const QList<Osm_Node*>::const_iterator it = m_way.get_nodes_list().cbegin();
	const QList<Osm_Node*>::const_iterator it_next = it;
	it_next++;

	for (auto it = m_edges.begin(); it != m_edges.end(); ++it) {
		scene()->removeItem(*it);
		removeFromGroup(*it);
		delete *it;
	}
	m_edges.clear();
	if (m_way.get_size() < 2) {
		return;
	}
	while (it_next != m_way.get_nodes_list().cend()) {
		m_edges.push_back(new Item_Edge(m_map, m_way, const_cast<Osm_Node*>(*it), const_cast<Osm_Node*>(*it_next)));
		addToGroup(m_edges.back());
		scene()->addItem(m_edges.back());
		it++;
		it_next++;
	}
}

void Item_Way::handle_event_update(Osm_Way& way) override {
	switch(get_meta()) {
	case NODE_ADDED:
		if (way.get_size() < 3) {
			compose_group();
			return;
		} else {
			handle_node_added();
		}
		break;
	case NODE_DELETED:
		if (way.get_size() < 3) {
			compose_group();
			return;
		} else {
			handle_node_deleted();
		}
		break;
	}
}
/*================================================================*/
/*                        Public methods                          */
/*================================================================*/

int Item_Way::type() const {
	return Type;
}

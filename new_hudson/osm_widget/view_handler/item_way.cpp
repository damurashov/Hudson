#include "item_way.h"
#include "view_handler.h"

using namespace ns_osm;

/*================================================================*/
/*                  Constructors, destructors                     */
/*================================================================*/

Item_Way::Item_Way(const Osm_Map& map,
                   View_Handler& view_handler,
                   Osm_Way& osm_way,
                   QGraphicsItem* p_parent)
                   : QGraphicsItemGroup(p_parent)
{
	QList<Edge>				edges = Edge::to_edge_list(m_way);
	QList<Edge>::iterator	it_edge;

	for (it_edge = edges.begin(); it_edge != edges.end(); ++it_edge) {
		m_edges.push_back(new Item_Edge(m_map, *it_edge, m_way));
		reg(m_edges.back());
	}

	subscribe(osm_way);
}

Item_Way::~Item_Way() {
	QList<Item_Edge*>::iterator it;

	for (it = m_edges.begin(); it != m_edges.end(); ++it) {
		unreg(*it);
		delete *it;
	}
}

/*================================================================*/
/*                       Private methods                          */
/*================================================================*/

bool Item_Way::split_edge(int item_edge_pos, Osm_Node *p_node_mid) {
	Osm_Node*					p_node_left;
	Osm_Node*					p_node_right;
	QList<Item_Edge*>::iterator it_item = m_edges.begin();
	Item_Edge*					p_prev_item;
	Item_Edge*					p_next_item;
	Item_Edge*					p_old_item;

	if (item_edge_pos < 0) {
		return false;
	}
	for (int i = 0; i < item_edge_pos && it_item != m_edges.end(); ++i) {
		it_item++;
	}
	if (it_item == m_edges.end()) {
		return false;
	}
	p_old_item = *it_item;
	p_node_left = p_old_item->first();
	p_node_right = p_old_item->second();
	p_prev_item = new Item_Edge(m_map, *p_node_left, *p_node_mid, m_way);
	p_next_item = new Item_Edge(m_map, *p_node_mid, *p_node_right, m_way);

	it_item = m_edges.insert(it_item, p_next_item);
	it_item = m_edges.insert(it_item, p_prev_item);
	m_edges.removeOne(p_old_item);

	reg(p_prev_item);
	reg(p_next_item);
	unreg(p_old_item);
	delete p_old_item;

	return true;
}

bool Item_Way::merge_edges(int pos_prev, int pos_next) {
	Osm_Node*					p_node_first;
	Osm_Node*					p_node_second;
	Item_Edge*					p_left_old_edge;
	Item_Edge*					p_right_old_edge;
	Item_Edge*					p_new_edge;
	QList<Item_Edge*>::iterator it_old_edge = m_edges.begin();

	if (pos_prev != pos_next - 1 || pos_prev < 0 || pos_next < 0) {
		return false;
	}
	for (int i = 0; i < pos_prev && it_old_edge != m_edges.end(); ++i) {
		p_left_old_edge = *it_old_edge;
		p_right_old_edge = *(++it_old_edge);
	}
	if (it_old_edge == m_edges.end()) {
		return false;
	}

	p_node_first = p_left_old_edge->first();
	p_node_second = p_right_old_edge->second();
	p_new_edge = new Item_Edge(m_map, *p_node_first, *p_node_second, m_way);

	m_edges.insert(it_old_edge, p_new_edge);
	m_edges.removeOne(p_left_old_edge);
	m_edges.removeOne(p_right_old_edge);

	reg(p_new_edge);
	unreg(p_left_old_edge);
	unreg(p_right_old_edge);
	delete p_left_old_edge;
	delete p_right_old_edge;

	return true;
}

int Item_Way::seek_pos_node_first(Osm_Node* p_node_left, Osm_Node* p_node_right) const {
	const QList<Osm_Node*>& nodes		= m_way.get_nodes_list();
	int						pos_left	= -1;
	int						pos_right	= -1;

	if (p_node_next == nullptr) {
		return nodes.indexOf(p_node_first);
	}
	while (pos_right != pos_left + 1) {
		pos_left = nodes.indexOf(p_node_left, pos_left + 1);
		if (pos_left < 0) {
			return -1;
		}
		pos_right = nodes.indexOf(p_node_right, pos_left);
		if (pos_right < 0) {
			return -1;
		}
	}

	return pos_left;
}

Item_Way::Diff Item_Way::get_diff() const {
	QList<Edge>							actual(Edge::to_edge_list(m_way));
	QList<Item_Edge*>::const_iterator	it_old_item = m_edges.cbegin();
	QList<Edge>::iterator				it_new_edge = actual.begin();
	Diff								diff;

	diff.type = Diff::NONE;
	/* seek diff */
	while (it_old_item != m_edges.cend() && it_new_edge != actual.end()) {
		it_old_item++;
		it_new_edge++;
	}
	/* detect diff type */
	if (it_old_item == m_edges.cend() && it_new_edge == actual.end()) {
		return diff;
	} else if (it_old_item == m_edges.cend()) {
		diff.type = Diff::ADD_BEFORE;
	} else if (it_new_edge == actual.end()) {
		diff.type = Diff::REMOVE;
	} else {
		QList<Edge>::iterator it_seeker = it_new_edge;
		while (!(**it_old_item == *it_seeker)) {
			it_seeker++;
		}
		diff.type = (it_seeker == actual.end() ? Diff::REMOVE : Diff::ADD_BEFORE);
	}
	/* compose diff */
	diff.it_diff = it_old_item;
	if (diff.type == Diff::ADD_BEFORE) {
		diff.p_node_first = it_new_edge->first();
		diff.p_node_second = it_new_edge->second();
	}

	return diff;
}

void Item_Way::handle_diffs() {
	Diff diff;
	Item_Edge* p_item_edge;

	while ((diff = get_diff()).type != Diff::NONE) {
		switch (diff.type) {
		case Diff::ADD_BEFORE:
			p_item_edge = new Item_Edge(m_map,
			                            *(diff.p_node_first),
			                            *(diff.p_node_second),
			                            m_way);
			reg(p_item_edge);
			m_edges.insert(diff.it_diff, p_item_edge);
			break;
		case Diff::REMOVE:
			m_edges.erase(diff.it_diff);
			p_item_edge = *(diff.it_diff);
			unreg(p_item_edge);
			delete p_item_edge;
			break;
		}
	}

	update();
}

void Item_Way::handle_added_front() {
	Osm_Node*	p_node_first;
	Osm_Node*	p_node_second;
	Item_Edge*	p_item_edge;

	if (m_way.get_size() < 2) {
		return;
	}
	p_node_first = const_cast<Osm_Node*>(*(m_way.get_nodes_list().cbegin()));
	p_node_second = const_cast<Osm_Node*>(*(m_way.get_nodes_list().cbegin()++));
	p_item_edge = new Item_Edge(m_map, *p_node_first, *p_node_second, m_way);
	reg(p_item_edge);
	m_edges.push_front(p_item_edge);
}

void Item_Way::handle_added_back() {
	Osm_Node*	p_node_first;
	Osm_Node*	p_node_second;
	Item_Edge*	p_item_edge;

	if (m_way.get_size() < 2) {
		return;
	}

	p_node_first = const_cast<Osm_Node*>(*(m_way.get_nodes_list().cend()--));
	p_node_second = const_cast<Osm_Node*>(*(m_way.get_nodes_list().cend()));
	p_item_edge = new Item_Edge(m_map, *p_node_first, *p_node_second, m_way);
	reg(p_item_edge);
	m_edges.push_back(p_item_edge);
}

void Item_Way::handle_added_mid(const Meta& meta) {
	QList<Item_Edge*>::iterator it_to_split;
	int							pos_node;
	int							pos_edge_to_split = -1;
	Osm_Node*					p_node;
	Osm_Node*					p_new_node;

	/* Seek edge pos */
	if ((pos_node = meta.get_pos(Meta::SUBJECT_AFTER)) >= 0) {
		pos_edge_to_split = pos_node;
	} else if ((pos_node = meta.get_pos(Meta::SUBJECT_BEFORE)) >= 0) {
		pos_edge_to_split = pos_node - 1;
	}
	if ((p_new_node = static_cast<Osm_Node*>(meta.get_subject())) == nullptr) {
		handle_diffs();
		return;
	}
	if ((p_node = static_cast<Osm_Node*>(meta.get_subject(Meta::SUBJECT_AFTER))) != nullptr) {
		pos_edge_to_split = seek_pos_node_first(p_node, p_new_node);
	} else if ((p_node = static_cast<Osm_Node*>(meta.get_subject(Meta::SUBJECT_BEFORE))) != nullptr) {
		pos_edge_to_split = seek_pos_node_first(p_new_node, p_node) - 1;
	}
	/* Split */
	if (!split_edge(pos_edge_to_split, p_new_node)) {
		handle_diffs();
	}
}

void Item_Way::handle_deleted_front() {
	Item_Edge* p_item_edge;

	if (m_way.get_size() == 0) {
		return;
	}

	p_item_edge = m_edges.front();
	unreg(p_item_edge);
	m_edges.pop_front();
	delete p_item_edge;
}

void Item_Way::handle_deleted_back() {
	Item_Edge* p_item_edge;

	if (m_way.get_size() == 0) {
		return;
	}

	p_item_edge = m_edges.back();
	unreg(p_item_edge);
	m_edges.pop_back();
	delete p_item_edge;
}

void Item_Way::handle_deleted_mid(const Meta& meta) {
	int			pos_edge_item_prev = -1;
	int			pos_edge_item_next = -1;
	int			pos_reference_node;
	Osm_Node*	p_del_node;
	Osm_Node*	p_reference_node;

	/* Seek edge pos */
	if ((pos_reference_node = meta.get_pos(Meta::SUBJECT_AFTER)) != -1) {
		pos_edge_item_prev = pos_reference_node;
		pos_edge_item_next = pos_edge_item_prev + 1;
	} else if ((pos_reference_node = meta.get_pos(Meta::SUBJECT_BEFORE)) != -1) {
		pos_edge_item_next = pos_reference_node - 1;
		pos_edge_item_prev = pos_edge_item_next - 1;
	}
	if ((p_del_node = static_cast<Osm_Node*>(meta.get_subject())) == nullptr) {
		handle_diffs();
		return;
	}
	if ((p_reference_node = static_cast<Osm_Node*>(meta.get_subject(Meta::SUBJECT_AFTER))) != nullptr) {
		pos_edge_item_prev = seek_pos_item_edge(p_reference_node, p_del_node);
		pos_edge_item_next = pos_edge_item_prev - 1;
	} else if ((p_reference_node = static_cast<Osm_Node*>(meta.get_subject(Meta::SUBJECT_BEFORE))) != nullptr) {
		pos_edge_item_next = seek_pos_item_edge(p_del_node, p_reference_node);
		pos_edge_item_prev = pos_edge_item_next - 1;
	}
	/* Merge */
	if (!merge_edges(pos_edge_item_prev, pos_edge_item_next)) {
		handle_diffs();
	}
}

void Item_Way::reg(Item_Edge* p_item) {
	if (p_item == nullptr) {
		return;
	}
//	scene()->addItem(p_item);
	if (scene()) {
		scene->addItem(p_item);
	}
	addToGroup(p_item);
	QObject::connect(p_item,
	                 SIGNAL(signal_edge_clicked(QPointF,Osm_Way*,Osm_Node*,Osm_Node*,Qt::MouseButton)),
	                 &m_view_handler,
	                 SLOT(slot_edge_clicked(QPointF,Osm_Way*,Osm_Node*,Osm_Node*,Qt::MouseButton)));
}

void Item_Way::unreg(Item_Edge* p_item) {
	if (p_item == nullptr) {
		return;
	}
	if (scene()) {
		scene()->removeItem(p_item);
	}
//	scene()->removeItem(p_item);
	removeFromGroup(p_item);
}

/*================================================================*/
/*                      Protected methods                         */
/*================================================================*/

void Item_Way::handle_event_update(Osm_Way& way) {
	Meta meta(get_meta());
	switch (meta) {
	case NODE_ADDED:
		if (meta.is_generic_event()) {
			handle_diffs();
		} else {
			switch (meta.get_event()) {
			case NODE_ADDED_FRONT:
				handle_added_front();
				break;
			case NODE_ADDED_BACK:
				handle_added_back();
				break;
			case NODE_ADDED_AFTER:
				handle_added_mid(meta);
				break;
			}
		}
		break;
	case NODE_DELETED:
		if (meta.is_generic_event()) {
			handle_diffs();
		} else {
			switch (meta.get_event()) {
			case NODE_DELETED_FRONT:
				handle_deleted_front();
				break;
			case NODE_DELETED_BACK:
				handle_deleted_back();
				break;
			case NODE_DELETED_AFTER:
				handle_deleted_mid(meta);
				break;
			}
		}
	}

	update();
}

/*================================================================*/
/*                        Public methods                          */
/*================================================================*/

void Item_Way::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	/* TODO: implement drawing */
}

int Item_Way::type() const {
	return Type;
}

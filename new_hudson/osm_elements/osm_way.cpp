#include "osm_way.h"
using namespace ns_osm;

/*================================================================*/
/*                  Constructors, destructors                     */
/*================================================================*/


Osm_Way::Osm_Way() : Osm_Object(Osm_Object::Type::WAY) {
	m_size = 0;
}

Osm_Way::Osm_Way(const QString &id)
    : Osm_Object(Osm_Object::Type::WAY),
      Osm_Info(id)
{
	m_size = 0;
}

Osm_Way::~Osm_Way() {
	emit_delete();
}

/*================================================================*/
/*                       Private methods                          */
/*================================================================*/

void Osm_Way::remove_all_entries(Osm_Node* p_node) {
	const long long				THIS_ID = get_inner_id();
	QList<Osm_Node*>::iterator	it = m_nodes.begin();
	unsigned int				pos_after = -1;
	Osm_Node*					p_after;

	unsubscribe(*p_node);
	m_set.remove(p_node);

	while (it != m_nodes.end()) {
		if ((*it) != p_node) {
			p_after = *it;
			pos_after++;
			it++;
		} else {
			m_size--;
			if ((*it) == m_nodes.front()) {
				m_nodes.pop_front();
				it = m_nodes.begin();
				emit_update(Meta(NODE_DELETED_FRONT).set_subject(*p_node));
				if (is_locked(THIS_ID)) {
					return;
				}
			} else if ((*it) == m_nodes.back()) {
				m_nodes.pop_back();
				emit_update(Meta(NODE_DELETED_BACK).set_subject(*p_node));
				return;
			} else {
				it = m_nodes.erase(it);
				emit_update(Meta(NODE_DELETED_AFTER)
				            .set_subject(*p_node)
				            .set_subject(*p_after, Meta::SUBJECT_AFTER)
				            .set_pos(pos_after, Meta::SUBJECT_AFTER));
				if (is_locked(THIS_ID)) {
					return;
				}
			}
		}
	}
}

/*================================================================*/
/*                      Protected methods                         */
/*================================================================*/

void Osm_Way::handle_event_delete(Osm_Node& node) {
	enum Situation_Type {
		MAKE_OPENED, /* The way is a triangle */
		SAVE_CLOSED, /* The way is not a triangle, and the deleting node is the first node of a circular way */
		REGULAR		 /* Any other case */
	}				type = REGULAR;
//	Event			event;
	const long long THIS_ID = get_inner_id();
//	unsigned short	pos = 0;

	if (is_closed()) {
		if (get_size() == 4) {
			type = MAKE_OPENED;
		} else if (&node == m_nodes.front()) {
			type = SAVE_CLOSED;
		}
	}

	remove_all_entries(&node);
	if (is_locked(THIS_ID)) {
		return;
	}
	switch (type) {
	case REGULAR:
		break;
	case SAVE_CLOSED:
		push_node(m_nodes.front());
		break;
	case MAKE_OPENED:
		if (is_closed()) {
			//m_nodes.push_back(m_nodes.front());
			m_size--;
			emit_update(Meta(NODE_DELETED_BACK).set_subject(*(m_nodes.takeLast())));
		}
		break;
	}
}

void Osm_Way::handle_event_update(Osm_Node& node) {
	emit_update(Meta(NODE_UPDATED).set_subject(node));
}

/*================================================================*/
/*                        Public methods                          */
/*================================================================*/


unsigned Osm_Way::get_size() const {
	return m_size;
}

unsigned short Osm_Way::get_capacity() const {
	return CAPACITY;
}

bool Osm_Way::push_node(Osm_Node* ptr_node) {
	const long long THIS_ID = get_inner_id();

	if (is_closed() || get_size() >= CAPACITY || ptr_node == nullptr) {
		set_valid(ptr_node == nullptr ? false : (ptr_node->is_valid() ? is_valid() : false));
		return false;
	} else if (has(ptr_node)) {
		if (ptr_node != m_nodes.front() || get_size() < 3) {
			return false;
		}
	}
	m_set.insert(ptr_node);
	m_nodes.push_back(ptr_node);
	m_size++;
	subscribe(*ptr_node);
	emit_update(Meta(NODE_ADDED_BACK).set_subject(*ptr_node));
	return (!is_locked(THIS_ID));
}

bool Osm_Way::insert_node_between(Osm_Node* p_node,
                                  Osm_Node* p_target_1,
                                  Osm_Node* p_target_2) {
	const long long				THIS_ID = get_inner_id();
	unsigned short				pos		= 0;
	QList<Osm_Node*>::iterator	it_node	= m_nodes.begin();
	Osm_Node*					p_after;

	if (p_node == nullptr
	        || !has(p_target_1)
	        || !has(p_target_2)
	        || has(p_node)) {
		return false;
	}
	while ((*it_node) != p_target_1 && (*it_node) != p_target_2) {
		it_node++;
		pos++;
	}

	p_after = *it_node;
	it_node++;
	if (it_node == m_nodes.end()) {
		return false;
	} else if ((*it_node) != p_target_1 && (*it_node) != p_target_2){
		return false;
	}
	m_nodes.insert(it_node, p_node);
	m_set.insert(p_node);
	m_size++;
	subscribe(*p_node);
	emit_update(Meta(NODE_ADDED_AFTER)
	            .set_subject(*p_node)
	            .set_subject(*p_after, Meta::SUBJECT_AFTER)
	            .set_pos(pos, Meta::SUBJECT_AFTER));
	return (!is_locked(THIS_ID));
}

bool Osm_Way::has(Osm_Node* ptr_node) const {
	if (ptr_node == nullptr) {
		return false;
	}
	return m_set.contains(ptr_node);
	//return m_nodes.contains(ptr_node);
}

bool Osm_Way::is_closed() const {
	if (get_size() > 2) {
		return (m_nodes.front() == m_nodes.back());
	}
	return false;
}

bool Osm_Way::is_empty() const {
	return (get_size() == 0);
}

const QList<Osm_Node*>& Osm_Way::get_nodes_list() const {
	return m_nodes;
}

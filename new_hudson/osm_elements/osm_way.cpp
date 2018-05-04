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
/*                      Protected methods                         */
/*================================================================*/

void Osm_Way::handle_event_delete(Osm_Node& node) {
	const long long				THIS_ID			= get_inner_id();
	unsigned short				pos				= 0;
	bool						f_stay_closed	= (is_closed() && (node.get_id() == m_nodes.back()->get_id()));
	QList<Osm_Node*>::iterator	it_node_prev;
	QList<Osm_Node*>::iterator	it_node_curr;

	if (&node == m_nodes.front()) {
		m_nodes.pop_front();
		m_size--;
		emit_update(Meta(NODE_DELETED_FRONT).set_subject(node));
		if (is_locked(THIS_ID)) {
			return;
		}
		if (m_nodes.isEmpty()) {
			return;
		}
	}

	it_node_prev = m_nodes.begin();
	it_node_curr = m_nodes.begin();
	if (it_node_curr == m_nodes.end()) {
		return;
	} else {
		it_node_curr++;
	}
	while (it_node_curr != m_nodes.end()) {
		if (&node == *it_node_curr) {
			it_node_curr = m_nodes.erase(it_node_curr);
			m_size--;
			emit_update(Meta(NODE_DELETED_AFTER)
			            .set_subject(node)
			            .set_subject(**it_node_prev, Meta::SUBJECT_AFTER)
			            .set_pos(pos, Meta::SUBJECT_AFTER));
			if (is_locked(THIS_ID)) {
				return;
			}
			if (it_node_curr == m_nodes.end()) {
				break;
			}
		}
		it_node_prev++;
		it_node_curr++;
		pos++;
	}
	if (f_stay_closed && get_size() > 2 && !m_nodes.isEmpty()) {
		//push_node(m_nodes.front());
		m_nodes.push_back(m_nodes.front());
		m_size++;
		emit_update(Meta(NODE_ADDED_BACK).set_subject(node));
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
	if (!is_closed() && !(get_size() >= CAPACITY)) {
		if (ptr_node == nullptr) {
			set_valid(false);
			return false;
		} else if (!m_nodes.isEmpty()) {
			if (m_nodes.back() == ptr_node) {
				return false;
			}
		}
		if (!(ptr_node->is_valid())) {
			set_valid(false);
		}
		m_nodes.push_back(ptr_node);
		m_size++;
		subscribe(*ptr_node);
		emit_update(Meta(NODE_ADDED_BACK).set_subject(*ptr_node));
		return true;
	}
	return false;
}

bool Osm_Way::insert_node_between(Osm_Node* node_ptr,
                                  Osm_Node* target_ptr_1,
                                  Osm_Node* target_ptr_2) {
	const long long THIS_ID = get_inner_id();
	unsigned short	pos		= 0;
	Osm_Node*		p_after;

	if (!has(node_ptr)
	&& node_ptr != nullptr
	&& target_ptr_1 != nullptr
	&& target_ptr_2 != nullptr) {
		QList<Osm_Node*>::iterator it;
		bool f_is_found = false;
		for (it = m_nodes.begin(); it != m_nodes.end(); ++it) {
			if ((*it == target_ptr_1) || (*it == target_ptr_2)) {
				f_is_found = true;
				break;
			}
			pos++;
		}
		if (f_is_found) {
			p_after = *it;
			it++;
			if (!(node_ptr->is_valid())) {
				set_valid(false);
			}
			m_nodes.insert(it, node_ptr);
			subscribe(*node_ptr);
			m_size++;
			if (m_nodes.back() == node_ptr/* && !is_closed()*/) {
				emit_update(Meta(NODE_ADDED_BACK).set_subject(*node_ptr));
				if (is_locked(THIS_ID)) {
					return true;
				}
			}
			emit_update(Meta(NODE_ADDED_AFTER)
			            .set_subject(*node_ptr)
			            .set_subject(*p_after, Meta::SUBJECT_AFTER)
			            .set_pos(pos, Meta::SUBJECT_AFTER));
			return true;
		}
	}
	return false;
}

bool Osm_Way::has(Osm_Node* ptr_node) const {
	if (ptr_node == nullptr) {
		return false;
	}
	return m_nodes.contains(ptr_node);
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

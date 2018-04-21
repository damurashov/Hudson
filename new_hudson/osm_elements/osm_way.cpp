#include "osm_way.h"
using namespace ns_osm;

/*================================================================*/
/*                  Constructors, destructors                     */
/*================================================================*/


Osm_Way::Osm_Way() : Osm_Object(Osm_Object::Type::WAY) {
	m_size = 0;
}

Osm_Way::Osm_Way(const QString &id) : Osm_Object(id, Osm_Object::Type::WAY) {
	m_size = 0;
}

Osm_Way::~Osm_Way() {
	emit_delete();
	/* Remove all orphaned nodes */
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
		emit_update();
		return true;
	}
	return false;
}

bool Osm_Way::insert_node_between(Osm_Node* node_ptr,
                                  Osm_Node* target_ptr_1,
                                  Osm_Node* target_ptr_2){
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
		}
		if (f_is_found) {
			it++;
			if (!(node_ptr->is_valid())) {
				set_valid(false);
			}
			m_nodes.insert(it, node_ptr);
			subscribe(*node_ptr);
			m_size++;
			emit_update();
			return true;
		}
	}
	return false;
}

bool Osm_Way::has(Osm_Node* ptr_node) const {
	if (ptr_node == nullptr) {
		return false;
	}
	return m_nodes.lastIndexOf(ptr_node) != -1;
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

void Osm_Way::handle_event_delete(Osm_Node& node) {
	bool f_stay_closed = (is_closed() && (node.get_id() == m_nodes.back()->get_id()));

	m_size -= m_nodes.removeAll(&node);
	if (f_stay_closed && get_size() >= 3) {
		push_node(m_nodes.front());
	}
}

void Osm_Way::handle_event_delete(Osm_Object&) {
	emit_update();
}

void Osm_Way::handle_event_update(Osm_Object&) {
	emit_update();
}

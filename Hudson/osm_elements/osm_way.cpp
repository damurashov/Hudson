#include "osm_way.h"

//======================= Constructors, destructors ==================
Osm_Way::Osm_Way() : Osm_Object() {
	m_size = 0;
	f_is_valid = true;
	f_is_closed = false;
}

Osm_Way::Osm_Way(const QString &id) : Osm_Object(id) {
	m_size = 0;
	f_is_valid = true;
	f_is_closed = false;
}

Osm_Way::~Osm_Way() {
}

//======================= Private methods ========================
void Osm_Way::handle_child_del(Osm_Object* ptr_child) {
	int ndel;
	Osm_Way* ptr_node = static_cast<Osm_Way*>(ptr_child);
	ndel = m_nodes.removeAll(ptr_node);
	m_size -= ndel;
}

//======================= Protected methods ========================
const Osm_Object::Type Osm_Way::get_type() const {
	return Osm_Object::Type::WAY;
}

//======================= Public methods ========================
Osm_Way* Osm_Way::get_by_id(const long long &way_id) const {
	return static_cast<Osm_Way*>(get_obj_by_id(Osm_Object::Type::WAY, id));
}

unsigned short Osm_Way::get_size() const {
	return m_size;
}

unsigned short Osm_Way::get_capacity() const {
	return CAPACITY;
}

void Osm_Way::push_node(Osm_Node* ptr_node) {
	if (!is_closed()
	        && !(m_size > CAPACITY)
	        && !(ptr_node == m_nodes.back())) {
		m_size++;
		if (ptr_node == nullptr) {
			f_is_valid = false;
		} else {
			reg_child(ptr_node);
			m_nodes.push_back(ptr_node);
			if (m_nodes.startsWith(ptr_node)) {
				f_is_closed = true;
			}
		}
	}
}

void Osm_Way::insert_node_between(Osm_Node* node_ptr,
                                  Osm_Node* target_ptr_1,
                                  Osm_Node* target_ptr_2){
	if (!has_node(node_ptr)
	&& node_ptr != nullptr
	&& target_ptr_1 != nullptr
	&& target_ptr_2 != nullptr) {
		QList<Osm_Node*>::iterator it;
		bool f_is_found = false;
		for (it = m_nodes.begin(); it != m_nodes.end(); ++it) {
			if ((it == target_ptr_1) || (it == target_ptr_2)) {
				f_is_found = true;
				break;
			}
		}
		if (f_is_found) {
			it++;
			m_nodes.insert(it, node_ptr);
			reg_child(node_ptr);
		}
	}
}

bool Osm_Way::has_node(Osm_Node* ptr_node) const {
	if (ptr_node == nullptr) {
		return false;
	}
	return get_children().contains(ptr_node->get_inner_id());
}

bool Osm_Way::is_closed() const {
	return f_is_closed;
}

bool Osm_Way::is_valid() const {
	return f_is_valid;
}

const QList<Osm_Node*>& Osm_Way::get_nodes_list() const {
	return m_nodes;
}

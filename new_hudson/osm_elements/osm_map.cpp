#include "osm_map.h"

using namespace ns_osm;
/*================================================================*/
/*                  Constructors, destructors                     */
/*================================================================*/

Osm_Map::Osm_Map() {
	mn_parents = 0;
	f_destruct_physically = true;
	f_remove_orphaned_nodes = true;
	f_remove_one_node_ways = true;
}

Osm_Map::~Osm_Map() {
	clear();
	emit_delete(MAP_DELETED);
}

/*================================================================*/
/*                       Private methods                          */
/*================================================================*/


void Osm_Map::handle_event_update(Osm_Node& node) {
	switch (get_meta()) {
	case NODE_UPDATED:
		emit_update(Meta().set_event(MAP_NODE_UPDATED).set_subject(node));
		break;
	}
}

void Osm_Map::handle_event_update(Osm_Way& way) {
	switch (get_meta()) {
	case NODE_ADDED:
		if (get_meta().get_subject() != nullptr) {
			Osm_Node* p_new_node = static_cast<Osm_Node*>(get_meta().get_subject());
			add(p_new_node);
			break;
		}
		for (auto it = way.get_nodes_list().cbegin(); it != way.get_nodes_list().cend(); ++it) {
			Osm_Node* p_node = const_cast<Osm_Node*>(*it);
			if (!has(p_node)) {
				add(p_node);
				break;
			}
		}
		break;
	case NODE_DELETED:
		if (way.get_size() == 1 && f_remove_one_node_ways) {
			set_remove_one_node_ways(false);
			Osm_Node* p_last_node = const_cast<Osm_Node*>(way.get_nodes_list().front());
			if (f_remove_orphaned_nodes && p_last_node->count_osm_subscribers() == 1) {
				if (f_destruct_physically) {
					set_remove_orphaned_nodes(false);
					way.unsubscribe(*p_last_node);
				}
				remove(p_last_node);
				set_remove_orphaned_nodes(true);
				//set_remove_one_node_ways(true);
			}
			remove(&way);
			set_remove_one_node_ways(true);
			//stop_broadcast();
		}
		break;
	}
}

void Osm_Map::handle_event_update(Osm_Relation& relation) {
	switch (get_meta()) {
	case NODE_ADDED:
		if (get_meta().get_subject() != nullptr) {
			Osm_Node* p_new_node = static_cast<Osm_Node*>(get_meta().get_subject());
			add(p_new_node);
			break;
		}
		for (auto it = relation.get_nodes().cbegin(); it != relation.get_nodes().cend(); ++it) {
			Osm_Node* p_node = const_cast<Osm_Node*>(*it);
			if (!has(p_node)) {
				add(p_node);
				//fit_autorects(*p_node);
				break;
			}
		}
		break;
	case WAY_ADDED:
		if (get_meta().get_subject() != nullptr) {
			Osm_Way* p_new_way = static_cast<Osm_Way*>(get_meta().get_subject());
			add(p_new_way);
			break;
		}
		for (auto it = relation.get_ways().cbegin(); it != relation.get_ways().cend(); ++it) {
			Osm_Way* p_way = const_cast<Osm_Way*>(*it);
			if (!has(p_way)) {
				add(p_way);
				break;
			}
		}
		break;
	case RELATION_ADDED:
		if (get_meta().get_subject() != nullptr) {
			Osm_Relation* p_new_relation = static_cast<Osm_Relation*>(get_meta().get_subject());
			add(p_new_relation);
			break;
		}
		for (auto it = relation.get_relations().cbegin(); it != relation.get_relations().cend(); ++it) {
			Osm_Relation* p_rel = const_cast<Osm_Relation*>(*it);
			if (!has(p_rel)) {
				add(p_rel);
				break;
			}
		}
	}
}

void Osm_Map::handle_event_delete(Osm_Node& node) {
	m_nodes_hash.remove(node.get_id());
}

void Osm_Map::handle_event_delete(Osm_Way& way) {
	bool f_buf_one_node_ways = f_remove_one_node_ways;
	QList<Osm_Node*> l_nodes_to_delete;

	m_ways_hash.remove(way.get_id());
	if (!f_remove_orphaned_nodes) {
		return;
	}
	set_remove_one_node_ways(false);
	for (auto it = way.get_nodes_list().cbegin(); it != way.get_nodes_list().cend(); ++it) {
		if ((*it)->count_osm_subscribers() == 1) {
			//remove(const_cast<Osm_Node*>(*it));
			l_nodes_to_delete.push_back(const_cast<Osm_Node*>(*it));
		}
	}
	for (auto it = l_nodes_to_delete.begin(); it != l_nodes_to_delete.end(); ++it) {
		remove(*it);
	}
	set_remove_one_node_ways(f_buf_one_node_ways);
}

void Osm_Map::handle_event_delete(Osm_Relation& rel) {
	m_relations_hash.remove(rel.get_id());
}


/*================================================================*/
/*                        Public methods                          */
/*================================================================*/

void Osm_Map::set_remove_physically(bool f) {
	f_destruct_physically = f;
}

void Osm_Map::set_remove_orphaned_nodes(bool f) {
	f_remove_orphaned_nodes = f;
}

void Osm_Map::set_remove_one_node_ways(bool f) {
	f_remove_one_node_ways = f;
}

void Osm_Map::set_bound(const QRectF& bound) {
	m_bounding_rect = bound;
}

int Osm_Map::count_parents() const {
	return mn_parents;
}

void Osm_Map::adopt() {
	mn_parents++;
}

void Osm_Map::orphan() {
	mn_parents--;
}

void Osm_Map::add(Osm_Node* p_node) {
	if (p_node != nullptr) {
		if (has(p_node)) {
			return;
		}
		if (!(p_node->is_valid())) {
			set_valid(false);
		}

		m_nodes_hash[p_node->get_id()] = p_node;
		subscribe(*p_node);
		emit_update(Meta(MAP_NODE_ADDED).set_subject(*p_node));
	}
}

void Osm_Map::add(Osm_Way* p_way) {
	if (p_way != nullptr) {
		if (has(p_way)) {
			return;
		}
		if (!(p_way->is_valid())) {
			set_valid(false);
		}
		for (auto it = p_way->get_nodes_list().cbegin(); it != p_way->get_nodes_list().cend(); ++it) {
			add(const_cast<Osm_Node*>(*it));
		}
		m_ways_hash[p_way->get_id()] = p_way;
		subscribe(*p_way);
		emit_update(Meta(MAP_WAY_ADDED).set_subject(*p_way));
	}
}

void Osm_Map::add(Osm_Relation* p_rel) {
	if (p_rel != nullptr) {
		if (has(p_rel)) {
			return;
		}
		if (!(p_rel->is_valid())) {
			set_valid(false);
		}
		for (auto it = p_rel->get_nodes().cbegin(); it != p_rel->get_nodes().cend(); ++it) {
			add(const_cast<Osm_Node*>(*it));
		}
		for (auto it = p_rel->get_ways().cbegin(); it != p_rel->get_ways().cend(); ++it) {
			add(const_cast<Osm_Way*>(*it));
		}
		for (auto it = p_rel->get_relations().cbegin(); it != p_rel->get_relations().cend(); ++it) {
			add(const_cast<Osm_Relation*>(*it));
		}
		m_relations_hash[p_rel->get_id()] = p_rel;
		subscribe(*p_rel);
		emit_update(Meta(MAP_RELATION_ADDED).set_subject(*p_rel));
	}
}

bool Osm_Map::has(Osm_Node* p_node) const {
	if (p_node == nullptr) {
		return false;
	}
	return m_nodes_hash.contains(p_node->get_id());
}

bool Osm_Map::has(Osm_Way* p_way) const {
	if (p_way == nullptr) {
		return false;
	}

	return m_ways_hash.contains(p_way->get_id());
}

bool Osm_Map::has(Osm_Relation* p_rel) const {
	if (p_rel == nullptr) {
		return false;
	}

	return m_relations_hash.contains(p_rel->get_id());
}

void Osm_Map::remove(Osm_Node* p_node) {
	if (p_node == nullptr) {
		return;
	}
	m_nodes_hash.remove(p_node->get_id());
	unsubscribe(*p_node);
	if (f_destruct_physically) {
		delete p_node;
	} else {
		p_node->emit_delete();
	}
}

void Osm_Map::remove(Osm_Way* p_way) {
	if (p_way == nullptr) {
		return;
	}
	m_ways_hash.remove(p_way->get_id());
	unsubscribe(*p_way);
	if (f_destruct_physically) {
		delete p_way;
	} else {
		p_way->emit_delete();
	}
}

void Osm_Map::remove(Osm_Relation* p_rel) {
	if (p_rel == nullptr) {
		return;
	}
	m_relations_hash.remove(p_rel->get_id());
	unsubscribe(*p_rel);
	if (f_destruct_physically) {
		delete p_rel;
	} else {
		p_rel->emit_delete();
	}
}

void Osm_Map::clear() {
	node_iterator it_node;
	way_iterator it_way;
	relation_iterator it_rel;

	unsubscribe();
	while ((it_rel = rbegin()) != rend()) {
		remove(*it_rel);
	}
	while ((it_way = wbegin()) != wend()) {
		remove(*it_way);
	}
	while ((it_node = nbegin()) != nend()) {
		remove(*it_node);
	}
	m_nodes_hash.clear();
	m_ways_hash.clear();
	m_relations_hash.clear();
	emit_update(MAP_CLEARED);
}

QRectF Osm_Map::get_bound() const {
	return m_bounding_rect;
}

Osm_Node* Osm_Map::get_node(long long id) {
	node_iterator it = m_nodes_hash.find(id);
	if (it == nend() || it.key() != id) {
		return nullptr;
	}
	return it.value();
}

Osm_Way* Osm_Map::get_way(long long id) {
	way_iterator it = m_ways_hash.find(id);
	if (it == wend() || it.key() != id) {
		return nullptr;
	}
	return it.value();
}

Osm_Relation* Osm_Map::get_relation(long long id) {
	relation_iterator it = m_relations_hash.find(id);
	if (it == rend() || it.key() != id) {
		return nullptr;
	}
	return it.value();
}

Osm_Map::node_iterator Osm_Map::nbegin() {
	return m_nodes_hash.begin();
}

Osm_Map::node_iterator Osm_Map::nend() {
	return m_nodes_hash.end();
}

Osm_Map::cnode_iterator Osm_Map::cnbegin() const {
	return m_nodes_hash.cbegin();
}

Osm_Map::cnode_iterator Osm_Map::cnend() const {
	return m_nodes_hash.cend();
}

Osm_Map::way_iterator Osm_Map::wbegin() {
	return m_ways_hash.begin();
}

Osm_Map::way_iterator Osm_Map::wend() {
	return m_ways_hash.end();
}

Osm_Map::cway_iterator Osm_Map::cwbegin() const {
	return m_ways_hash.cbegin();
}

Osm_Map::cway_iterator Osm_Map::cwend() const {
	return m_ways_hash.cend();
}

Osm_Map::relation_iterator Osm_Map::rbegin() {
	return m_relations_hash.begin();
}

Osm_Map::relation_iterator Osm_Map::rend() {
	return m_relations_hash.end();
}

Osm_Map::crelation_iterator Osm_Map::crbegin() const {
	return m_relations_hash.cbegin();
}

Osm_Map::crelation_iterator Osm_Map::crend() const {
	return m_relations_hash.cend();
}

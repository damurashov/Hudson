#include "osm_map.h"

using namespace ns_osm;

/*================================================================*/
/*                  Constructors, destructors                     */
/*================================================================*/

Osm_Map::Osm_Map() {
	const double BIG = 300.0;
	mn_parents = 0;
	f_destruct_physically = true;
	f_remove_orphaned_nodes = true;
	f_is_valid = true;
	m_autorect_normal.setLeft(BIG);
	m_autorect_normal.setRight(-BIG);
	m_autorect_normal.setTop(-BIG);
	m_autorect_normal.setBottom(BIG);
	m_autorect_180.setLeft(BIG);
	m_autorect_180.setRight(-BIG);
	m_autorect_180.setTop(-BIG);
	m_autorect_180.setBottom(BIG);
}

Osm_Map::~Osm_Map() {
	clear();
}

/*================================================================*/
/*                        Public methods                          */
/*================================================================*/

bool Osm_Map::is_valid() const {
	return f_is_valid;
}

void Osm_Map::set_remove_physically(bool f) {
	f_destruct_physically = f;
}

void Osm_Map::set_remove_orphaned_nodes(bool f) {
	f_remove_orphaned_nodes = f;
}

void Osm_Map::set_bound(const QRectF& bound) {
	m_user_rect = bound;
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
	double lon;
	double lat;

	if (p_node != nullptr) {
		if (has(p_node)) {
			return;
		}
		lon = p_node->get_lon();
		lat = p_node->get_lat();
		if (!(p_node->is_valid())) {
			f_is_valid = false;
		}
		/* Mutual vertical bounds, ... */
		if (lat > m_autorect_normal.top()) {
			m_autorect_normal.setTop(lat);
			m_autorect_180.setTop(lat);
		}
		if (lat < m_autorect_normal.bottom()) {
			m_autorect_normal.setBottom(lat);
			m_autorect_180.setBottom(lat);
		}
		/* ... but different horisontal ones */
		if (lon < m_autorect_normal.left()) {
			m_autorect_normal.setLeft(lon);
		}
		if (lon > m_autorect_normal.right()) {
			m_autorect_normal.setRight(lon);
		}
		if (lon < m_autorect_180.left() && lon > 0) {
			m_autorect_180.setLeft(lon);
		}
		if (lon > m_autorect_180.right() && lon <= 0) {
			m_autorect_180.setRight(lon);
		}
		m_nodes_hash[p_node->get_id()] = p_node;
		subscribe(*p_node);
	}
}

void Osm_Map::add(Osm_Way* p_way) {
	if (p_way != nullptr) {
		if (has(p_way)) {
			return;
		}
		if (!(p_way->is_valid())) {
			f_is_valid = false;
		}
		for (auto it = p_way->get_nodes_list().cbegin(); it != p_way->get_nodes_list().cend(); ++it) {
			add(const_cast<Osm_Node*>(*it));
		}
		m_ways_hash[p_way->get_id()] = p_way;
		subscribe(*p_way);
	}
}

void Osm_Map::add(Osm_Relation* p_rel) {
	if (p_rel != nullptr) {
		if (has(p_rel)) {
			return;
		}
		if (!(p_rel->is_valid())) {
			f_is_valid = false;
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
	m_nodes_hash.remove(p_node->get_id());
	unsubscribe(*p_node);
	if (f_destruct_physically) {
		delete p_node;
	}
}

void Osm_Map::remove(Osm_Way* p_way) {
	m_ways_hash.remove(p_way->get_id());
	unsubscribe(*p_way);
	if (f_destruct_physically) {
		delete p_way;
	}
}

void Osm_Map::remove(Osm_Relation* p_rel) {
	m_relations_hash.remove(p_rel->get_id());
	unsubscribe(*p_rel);
	if (f_destruct_physically) {
		delete p_rel;
	}
}

void Osm_Map::clear() {
	unsubscribe();
	for (auto it = nbegin(); it != nend(); ++it) {
		//unsubscribe(**it);
		if (f_destruct_physically) {
			delete *it;
		}
	}
	for (auto it = wbegin(); it != wend(); ++it) {
		//unsubscribe(**it);
		if (f_destruct_physically) {
			delete *it;
		}
	}
	for (auto it = rbegin(); it != rend(); ++it) {
		//unsubscribe(**it);
		if (f_destruct_physically) {
			delete *it;
		}
	}
	m_nodes_hash.clear();
	m_ways_hash.clear();
	m_relations_hash.clear();
}

void Osm_Map::fit_bounding_rect() {
//	long long longitude;

//	if (m_user_rect.width()!=0 && m_user_rect.height()!=0) {
//		return;
//	}
}

QRectF Osm_Map::get_bound(bool f_force_precalculated) const {
	double width_normal;
	double width_180;

	if (m_user_rect.width() == 0 || m_user_rect.height() == 0 || f_force_precalculated) {
		width_normal = m_autorect_normal.right() - m_autorect_normal.left();
		width_180 = m_autorect_180.left() - m_autorect_180.right();

		if (width_180 < width_normal) {
			return m_autorect_180;
		} else {
			return m_autorect_normal;
		}
	}
	return m_user_rect;
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

void Osm_Map::handle_event_delete(Osm_Node& node) {
	m_nodes_hash.remove(node.get_id());
}

void Osm_Map::handle_event_delete(Osm_Way& way) {
	m_ways_hash.remove(way.get_id());

	/* Remove orphaned nodes */
	if (!f_remove_orphaned_nodes) {
		return;
	}
	for (auto it = nbegin(); it != nend(); ++it) {
		if ((*it)->count_subscribers() == 0) {
			remove(*it);
		}
	}
}

void Osm_Map::handle_event_delete(Osm_Relation& rel) {
	m_relations_hash.remove(rel.get_id());
}

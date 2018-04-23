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
	f_force_dynamic_bound = false;

	m_bounding_rect.setLeft(0.0);
	m_bounding_rect.setRight(0.0);
	m_bounding_rect.setTop(0.0);
	m_bounding_rect.setBottom(0.0);
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
/*                       Private methods                          */
/*================================================================*/

bool Osm_Map::is_valid_bound(const QRectF& rect) const {
	if (rect.width() == 0.0 || rect.height() == 0.0) {
		return false;
	}
	if (std::abs(rect.bottom()) > 90 || std::abs(rect.top()) > 90 || rect.bottom() > rect.top()) {
		return false;
	}
	if (std::abs(rect.left()) >= 180 || std::abs(rect.right()) > 180) {
		return false;
	}
	return true;
}

bool Osm_Map::has_issue_180(const QRectF& rect) const {
	return rect.right() < rect.left();
}

void Osm_Map::fit_autorects(Osm_Node& node) {
	double lat = node.get_lat();
	double lon = node.get_lon();

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
}

/*================================================================*/
/*                        Public methods                          */
/*================================================================*/

bool Osm_Map::is_valid() const {
	return f_is_valid;
}

bool Osm_Map::includes_in_scene(Osm_Node* p_node) const {
	if (p_node == nullptr) {
		return false;
	}

	return get_scene_rect().contains(get_scene_coord(p_node));
}

void Osm_Map::set_remove_physically(bool f) {
	f_destruct_physically = f;
}

void Osm_Map::set_remove_orphaned_nodes(bool f) {
	f_remove_orphaned_nodes = f;
}

void Osm_Map::set_force_use_dynamic_bound(bool f) {
	f_force_dynamic_bound = f;
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
//	double lon;
//	double lat;

	if (p_node != nullptr) {
		if (has(p_node)) {
			return;
		}
//		lon = p_node->get_lon();
//		lat = p_node->get_lat();
		if (!(p_node->is_valid())) {
			f_is_valid = false;
		}
		fit_autorects(*p_node);

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

//	if (m_bounding_rect.width()!=0 && m_bounding_rect.height()!=0) {
//		return;
//	}
}

QRectF Osm_Map::get_bound() const {
	double width_normal;
	double width_180;

	if (!is_valid_bound(m_bounding_rect) || f_force_dynamic_bound) {
		width_normal = m_autorect_normal.right() - m_autorect_normal.left();
		width_180 = m_autorect_180.left() - m_autorect_180.right();

		if (width_180 < width_normal) {
			return m_autorect_180;
		} else {
			return m_autorect_normal;
		}
	}
	return m_bounding_rect;
}

QRectF Osm_Map::get_scene_rect() const {
	QRectF rect = get_bound();
	if (has_issue_180(rect)) {
		rect.setRight(rect.right() + 360.0);
	}
	return rect;
}

QPointF Osm_Map::get_scene_coord(Osm_Node* p_node) const {
	QPointF point;

	if (!has_issue_180(get_bound()) || p_node->get_lon() > 0) {
		point.setX(p_node->get_lon());
		point.setY(p_node->get_lat());
	} else {
		point.setX(p_node->get_lon() + 360.0);
		point.setY(p_node->get_lat());
	}

	return point;
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

void Osm_Map::handle_event_update(Osm_Node& node) {
	switch (get_meta()) {
	case NODE_UPDATED:
		fit_autorects(node);
		break;
	}
}

void Osm_Map::handle_event_update(Osm_Way& way) {
	switch (get_meta()) {
	case NODE_ADDED:
		for (auto it = way.get_nodes_list().cbegin(); it != way.get_nodes_list().cend(); ++it) {
			Osm_Node* p_node = const_cast<Osm_Node*>(*it);
			if (!has(p_node)) {
				add(p_node);
				break;
			}
		}
		break;
	}
}

void Osm_Map::handle_event_update(Osm_Relation& relation) {
	switch (get_meta()) {
	case NODE_ADDED:
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
		for (auto it = relation.get_ways().cbegin(); it != relation.get_ways().cend(); ++it) {
			Osm_Way* p_way = const_cast<Osm_Way*>(*it);
			if (!has(p_way)) {
				add(p_way);
				break;
			}
		}
		break;
	case RELATION_ADDED:
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

#include "osm_map.h"

using namespace ns_osm;

/*================================================================*/
/*                  Constructors, destructors                     */
/*================================================================*/

Osm_Map::Osm_Map() : mn_parents(0) {}

/*================================================================*/
/*                        Public methods                          */
/*================================================================*/

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
		m_nodes_hash[p_node->get_id()] = p_node;
	}
}

void Osm_Map::add(Osm_Way* p_way) {
	if (p_way != nullptr) {
		m_ways_hash[p_way->get_id()] = p_way;
	}
}

void Osm_Map::add(Osm_Relation* p_rel) {
	if (p_rel != nullptr) {
		m_relations_hash[p_rel->get_id()] = p_rel;
	}
}

void Osm_Map::remove(Osm_Node* p_node) {
	m_nodes_hash.remove(p_node->get_id());
	delete p_node;
}

void Osm_Map::remove(Osm_Way* p_way) {
	m_ways_hash.remove(p_way->get_id());
	delete p_way;
}

void Osm_Map::remove(Osm_Relation* p_rel) {
	m_relations_hash.remove(p_rel->get_id());
	delete p_relation;
}

void Osm_Map::clear() {
	for (auto it = nbegin(); it != nend(); ++it) {
		delete *it;
	}
	for (auto it = wbegin(); it != wend(); ++it) {
		delete *it;
	}
	for (auto it = rbegin(); it != rend(); ++it) {
		delete *it;
	}
	m_nodes_hash.clear();
	m_ways_hash.clear();
	m_relations_hash.clear();
}

void Osm_Map::fit_bounding_rect() {
	node_iterator it;
	long long minlat = 90;
	long long minlon = 180;
	long long maxlat = -90;
	long long maxlon = -180;
	long long temp_lat;
	long long temp_lon;

	if (m_bounding_rect.width()!=0 && m_bounding_rect.height()!=0) {
		return;
	}
	if (!m_nodes_hash.isEmpty()) {
		for (auto it = nbegin(); it !=nend(); ++it) {
			temp_lat = it.value()->get_lat();
			temp_lat = it.value()->get_lon();

			if (temp_lat < minlat) {
				minlat = temp_lat;
			}
			if (temp_lat > maxlat) {
				maxlat = temp_lat;
			}
			if (temp_lon < minlon) {
				minlon = temp_lon;
			}
			if (temp_lon > maxlon) {
				maxlon = temp_lon;
			}
		}
		m_bounding_rect.setLeft(minlon);
		m_bounding_rect.setBottom(minlat);
		m_bounding_rect.setRight(maxlon);
		m_bounding_rect.setTop(maxlat);
	}
	handle_lon_180();
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

Osm_Map::cway_iterator Osm_Map::сwbegin() const {
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

/*================================================================*/
/*                       Private methods                          */
/*================================================================*/

void Osm_Map::handle_lon_180() {
	if (m_bounding_rect.right() - m_bounding_rect.left() > 180) {
		qreal oldleft = m_bounding_rect.left();
		m_bounding_rect.setLeft(m_bounding_rect.right());
		m_bounding_rect.setRight(180-oldleft);
	}
}

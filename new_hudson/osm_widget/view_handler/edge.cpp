#include "edge.h"

using namespace ns_osm;

/*================================================================*/
/*                        Static members                          */
/*================================================================*/

//long long Edge::s_id_bound = 0xFFFFFFFFFFFFFFFF;

/*================================================================*/
/*          Constructors, destructors, arithmetic ops.            */
/*================================================================*/

Edge::Edge(Osm_Node& first, Osm_Node& second) /*: ID(s_id_bound++)*/ {
	mp_node_first = &first;
	mp_node_second = &second;
}

Edge::Edge(const Edge& edge) /*: ID(edge.ID)*/ {
	mp_node_first = edge.mp_node_first;
	mp_node_second = edge.mp_node_second;
}

Edge& Edge::operator=(const Edge& edge) {
	mp_node_first = edge.mp_node_first;
	mp_node_second = edge.mp_node_second;
	return *this;
}

Edge::Edge(Edge&& edge) /*: ID(edge.id)*/ {
	mp_node_first = edge.mp_node_first;
	mp_node_second = edge.mp_node_second;
}

Edge::~Edge() {}

/*================================================================*/
/*                        Public methods                          */
/*================================================================*/

//long long Edge::get_id() const {
//	return ID;
//}

QList<Edge> Edge::to_edge_list(const Osm_Way& way) {
	return to_edge_list(way.get_nodes_list());
}

QList<Edge> Edge::to_edge_list(const QList<Osm_Node*>& nodelist) {
	QList<Edge>					edges;
	QList<Osm_Node*>			nodes(nodelist);
	QList<Osm_Node*>::iterator	it_current = nodes.begin();
	QList<Osm_Node*>::iterator	it_next = nodes.begin();

	it_next++;
	while (it_next != nodes.end()) {
		edges.push_back(Edge(**it_current, **it_next));
		it_current++;
		it_next++;
	}

	return edges;
}

Osm_Node* Edge::first() const {
	return mp_node_first;
}

Osm_Node* Edge::second() const {
	return mp_node_second;
}

/*================================================================*/
/*                           Friends                              */
/*================================================================*/

bool ns_osm::operator==(const ns_osm::Edge& lhs, const ns_osm::Edge& rhs) {
	return ((lhs.first() == rhs.first() && lhs.second() == rhs.second())
	        || (lhs.first() == rhs.second() && lhs.second() == rhs.first()));
}

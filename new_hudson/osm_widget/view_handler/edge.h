#ifndef EDGE_H
#define EDGE_H

#include "osm_elements.h"
//#include "item_edge.h"

namespace ns_osm {

class Edge {
private:
//	static long long	s_id_bound;
//	long long			ID;
	Osm_Node*			mp_node_first;
	Osm_Node*			mp_node_second;
public:
//	long long			get_id			() const;
	static QList<Edge>	to_edge_list	(const Osm_Way&);
	static QList<Edge>	to_edge_list	(const QList<Osm_Node*>&);
	Osm_Node*			first			() const;
	Osm_Node*			second			() const;
	                    Edge			(Osm_Node& p_first, Osm_Node& p_second);
						Edge			(const Edge&);
						Edge			(Edge&&);
						Edge& operator=	(const Edge&);
	friend bool			operator==		(const Edge&, const Edge&);
	virtual				~Edge();
	                    Edge() = delete;
};

}
#endif // EDGE_H

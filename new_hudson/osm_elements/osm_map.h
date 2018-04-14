#ifndef OSM_MAP_H
#define OSM_MAP_H

#include "osm_node.h"
#include "osm_way.h"
#include "osm_relation.h"

class Osm_Map {
private:
	int										mn_parents;
	QHash<long long, ns_osm::Osm_Node*>		m_nodes_hash;
	QHash<long long, ns_osm::Osm_Way*>		m_ways_hash;
	QHash<long long, ns_osm::Osm_Relation*> m_relations_hash;
	QRectF									m_bounding_rect;

	void									handle_lon_180			();
	                                        Osm_Map					(const Osm_Map&) = delete;
	Osm_Map&								operator=				(const Osm_Map&) = delete;
public:
	typedef QHash<long long, ns_osm::Osm_Node*>::iterator			node_iterator;
	typedef QHash<long long, ns_osm::Osm_Way*>::iterator			way_iterator;
	typedef QHash<long long, ns_osm::Osm_Relation*>::iterator		relation_iterator;
	typedef QHash<long long, ns_osm::Osm_Node*>::const_iterator		cnode_iterator;
	typedef QHash<long long, ns_osm::Osm_Way*>::const_iterator		cway_iterator;
	typedef QHash<long long, ns_osm::Osm_Relation*>::const_iterator	crelation_iterator;

	int										count_parents			() const;
	void									set_bound				(const QRectF&);
	void									adopt					();
	void									orphan					();
	void									add						(ns_osm::Osm_Node*);
	void									add						(ns_osm::Osm_Way*);
	void									add						(ns_osm::Osm_Relation*);
	void									remove					(ns_osm::Osm_Node*);
	void									remove					(ns_osm::Osm_Way*);
	void									remove					(ns_osm::Osm_Relation*);
	void									clear					();
	void									fit_bounding_rect		();
	QRectF									get_bound				() const;
	ns_osm::Osm_Node*						get_node				(long long id_node);
	ns_osm::Osm_Way*						get_way					(long long id_way);
	ns_osm::Osm_Relation*					get_relation			(long long id_relation);
	node_iterator							nbegin					();
	node_iterator							nend					();
	cnode_iterator							cnbegin					() const;
	cnode_iterator							cnend					() const;
	way_iterator							wbegin					();
	way_iterator							wend					();
	cway_iterator							cwbegin					();
	cway_iterator							cwend					() const;
	relation_iterator						rbegin					();
	relation_iterator						rend					();
	crelation_iterator						crbegin					() const;
	crelation_iterator						crend					() const;
	                                        Osm_Map();
};

#endif // OSM_MAP_H

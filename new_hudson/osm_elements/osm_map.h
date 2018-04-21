#ifndef OSM_MAP_H
#define OSM_MAP_H

#include "osm_object.h"
#include "osm_node.h"
#include "osm_way.h"
#include "osm_relation.h"

namespace ns_osm {

class Osm_Map : public Osm_Subscriber {
private:
	int										mn_parents;
	QHash<long long, ns_osm::Osm_Node*>		m_nodes_hash;
	QHash<long long, ns_osm::Osm_Way*>		m_ways_hash;
	QHash<long long, ns_osm::Osm_Relation*> m_relations_hash;
	QRectF									m_bounding_rect;
	bool									f_destruct_physically;
	bool									f_remove_orphaned_nodes;
	bool									f_is_valid;
	bool									f_has_lon180_issue;
	double									m_minlon;
	double									m_maxlon;
	double									m_minlat;
	double									m_maxlat;

	                                        Osm_Map						(const Osm_Map&) = delete;
	Osm_Map&								operator=					(const Osm_Map&) = delete;
public:
	typedef QHash<long long, ns_osm::Osm_Node*>::iterator				node_iterator;
	typedef QHash<long long, ns_osm::Osm_Way*>::iterator				way_iterator;
	typedef QHash<long long, ns_osm::Osm_Relation*>::iterator			relation_iterator;
	typedef QHash<long long, ns_osm::Osm_Node*>::const_iterator			cnode_iterator;
	typedef QHash<long long, ns_osm::Osm_Way*>::const_iterator			cway_iterator;
	typedef QHash<long long, ns_osm::Osm_Relation*>::const_iterator		crelation_iterator;

	bool									is_valid					() const;
	void									set_remove_physically		(bool f);
	void									set_remove_orphaned_nodes	(bool f);
	int										count_parents				() const;
	void									set_bound					(const QRectF&);
	void									adopt						();
	void									orphan						();
	void									add							(ns_osm::Osm_Node*);
	void									add							(ns_osm::Osm_Way*);
	void									add							(ns_osm::Osm_Relation*);
	bool									has							(ns_osm::Osm_Node*) const;
	bool									has							(ns_osm::Osm_Way*) const;
	bool									has							(ns_osm::Osm_Relation*) const;
	void									remove						(ns_osm::Osm_Node*);
	void									remove						(ns_osm::Osm_Way*);
	void									remove						(ns_osm::Osm_Relation*);
	void									clear						();
	void									fit_bounding_rect			();
	QRectF									get_bound					() const;
	ns_osm::Osm_Node*						get_node					(long long id_node);
	ns_osm::Osm_Way*						get_way						(long long id_way);
	ns_osm::Osm_Relation*					get_relation				(long long id_relation);
	node_iterator							nbegin						();
	node_iterator							nend						();
	cnode_iterator							cnbegin						() const;
	cnode_iterator							cnend						() const;
	way_iterator							wbegin						();
	way_iterator							wend						();
	cway_iterator							cwbegin						() const;
	cway_iterator							cwend						() const;
	relation_iterator						rbegin						();
	relation_iterator						rend						();
	crelation_iterator						crbegin						() const;
	crelation_iterator						crend						() const;
	void									handle_event_delete			(Osm_Node&) override;
	void									handle_event_delete			(Osm_Way&) override;
	void									handle_event_delete			(Osm_Relation&) override;
	                                        Osm_Map						();
											~Osm_Map					();
};

}

#endif // OSM_MAP_H

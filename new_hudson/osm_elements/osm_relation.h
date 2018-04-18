#ifndef OSM_RELATION_H
#define OSM_RELATION_H

#include "osm_object.h"
#include "osm_node.h"
#include "osm_way.h"

namespace ns_osm {

class Osm_Relation : public Osm_Object, public Osm_Subscriber {
private:
	unsigned short							mn_nodes;
	unsigned short							mn_ways;
	unsigned short							mn_relations;
	QHash<long long, QString>				m_roles_hash;
	QList<Osm_Node*>						m_nodes_list;
	QList<Osm_Way*>							m_ways_list;
	QList<Osm_Relation*>					m_relations_list;
public:
	void									add					(Osm_Node*, const QString& role = "");
	void									add					(Osm_Way*, const QString& role = "");
	void									add					(Osm_Relation*, const QString& role = "");
	void									remove				(Osm_Node*);
	void									remove				(Osm_Way*);
	void									remove				(Osm_Relation*);
	bool									has					(Osm_Object*) const;
	void									set_role			(Osm_Object*, const QString& role);
	unsigned								get_size			() const;
	const QString							get_role			(Osm_Object*) const;
	const QList<Osm_Node*>&					get_nodes			() const;
	const QList<Osm_Way*>&					get_ways			() const;
	const QList<Osm_Relation*>&				get_relations		() const;
	unsigned short							count_nodes			() const;
	unsigned short							count_ways			() const;
	unsigned short							count_relations		() const;
	void									handle_event_delete	(Osm_Node&) override;
	void									handle_event_delete	(Osm_Way&) override;
	void									handle_event_delete	(Osm_Relation&) override;
	void									handle_event_delete	(Osm_Object&) override;
	void									handle_event_update	(Osm_Object&) override;
	                                        Osm_Relation		(const QString& id);
											Osm_Relation		();
	virtual									~Osm_Relation		();
};
}
#endif // OSM_RELATION_H

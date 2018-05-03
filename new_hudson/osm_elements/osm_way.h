#ifndef OSM_WAY_H
#define OSM_WAY_H

#include "osm_subscriber.h"
#include "osm_object.h"
#include "osm_node.h"
#include "osm_info.h"

namespace ns_osm {

class Osm_Way : public Osm_Object, public Osm_Subscriber, public Osm_Info {
private:
	static const unsigned short				CAPACITY = 2000;
	unsigned								m_size;
	QList<Osm_Node*>						m_nodes;
protected:
	void									handle_event_delete	(Osm_Node&) override;
	void									handle_event_update	(Osm_Node&) override;
public:
	unsigned								get_size			() const;
	unsigned short							get_capacity		() const;
	bool									push_node			(Osm_Node*);
	bool									insert_node_between	(Osm_Node* ptr_node,
																 Osm_Node* ptr_target_1,
																 Osm_Node* ptr_target_2);
	bool									has					(Osm_Node*) const;
	bool									is_closed			() const;
	bool									is_empty			() const;
	const QList<Osm_Node*>&					get_nodes_list		() const;
	//void									handle_event_update	(Osm_Object&) override;
	//void									handle_event_delete	(Osm_Object&) override;
											Osm_Way				(const QString& id);
											Osm_Way				();
											Osm_Way				(const Osm_Way&) = delete;
	Osm_Way&								operator=			(const Osm_Way&) = delete;
	virtual									~Osm_Way			();
};

}
#endif // OSM_WAY_H

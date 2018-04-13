#ifndef OSM_WAY_H
#define OSM_WAY_H

#include "osm_object.h"
#include "osm_node.h"

namespace ns_osm {

class Osm_Way : public Osm_Object {
private:
	static const unsigned short				CAPACITY = 2000;
	unsigned								m_size;
	QList<Osm_Node*>						m_nodes;

	void									handle_child_del	(Osm_Object* ptr_child) override;
public:
	unsigned								get_size			() const;
	unsigned short							get_capacity		() const;
	bool									push_node			(Osm_Node*);
	bool									insert_node_between	(Osm_Node* ptr_node,
																 Osm_Node* ptr_target_1,
																 Osm_Node* ptr_target_2);
	bool									has_node			(Osm_Node*) const;
	bool									is_closed			() const;
	bool									is_empty			() const;
	const QList<Osm_Node*>&					get_nodes_list		() const;
											Osm_Way				(const QString& id);
											Osm_Way				();
	virtual									~Osm_Way			();
};

}
#endif // OSM_WAY_H

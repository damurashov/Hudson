#ifndef OSM_WAY_H
#define OSM_WAY_H

#include "osm_object.h"
#include "osm_node.h"

class Osm_Way : public Osm_Object {
private:
	static const unsigned			CAPACITY = 2000;
	bool							f_is_valid;
	bool							f_is_closed;
	unsigned short					m_size;
	QList<Osm_Node*>				m_nodes;

	virtual void					handle_child_del	(Osm_Object* ptr_child);
protected:
	virtual const Osm_Object::Type	get_type			() const;
public:
	static Osm_Way*					get_by_id			(const long long &way_id) const;
	unsigned short					get_size			() const;
	unsigned short					get_capacity		() const;
	void							push_node			(Osm_Node*);
	void 							insert_node_between	(Osm_Node* node,
	                                                     Osm_Node* target_ptr_1,
	                                                     Osm_Node* target_ptr_2);
	bool							has_node			(Osm_Node*) const;
	bool							is_closed			() const;
	virtual bool					is_valid			() const;
	const QList<Osm_Node*>&			get_nodes_list		() const;
	                                Osm_Way				(const QString& id);
									Osm_Way				();
	virtual							~Osm_Way			();
};

#endif // OSM_WAY_H

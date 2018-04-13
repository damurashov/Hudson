#ifndef OSM_NODE_H
#define OSM_NODE_H

#include "osm_object.h"

class Osm_Node : public Osm_Object {
private:
	double							m_lat;
	double							m_lon;

	virtual void					handle_child_del(Osm_Object* child);
	                                Osm_Node		();
protected:
	virtual const Osm_Object::Type	get_type		() const;
public:
	static Osm_Node*				get_by_id		(const long long &id) const;
	virtual bool					is_valid		() const;
	double							get_lat			() const;
	double							get_lon			() const;
	void							set_lat			(const double& latitude);
	void							set_lon			(const double& longitude);
	                                Osm_Node		(const QString& latitude,
									                 const QString& longitude);
	                                Osm_Node		(const QString& id,
									                 const QString& latitude,
									                 const QString& longitude);
									Osm_Node		(const double& latitude, const double& longitude);
	virtual							~Osm_Node		();
};

#endif // OSM_NODE_H

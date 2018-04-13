#ifndef OSM_NODE_H
#define OSM_NODE_H

#include "osm_object.h"

namespace ns_osm {

class Osm_Node : public Osm_Object {
private:
	double		m_lat;
	double		m_lon;

	void		handle_child_del(Osm_Object* child) override;
	            Osm_Node		();
public:
	double		get_lat			() const;
	double		get_lon			() const;
	void		set_lat			(const double& latitude);
	void		set_lon			(const double& longitude);
	            Osm_Node		(const QString& latitude,
				                 const QString& longitude);
				Osm_Node		(const QString& id,
				                 const QString& latitude,
				                 const QString& longitude);
				Osm_Node		(const double& latitude, const double& longitude);
	virtual		~Osm_Node		();
};

}/* namespace ns_osm */

#endif // OSM_NODE_H

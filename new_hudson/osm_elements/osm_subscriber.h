#ifndef OSM_SUBSCRIBER_H
#define OSM_SUBSCRIBER_H

#ifndef QT_CORE_H
#define QT_CORE_H
#include <QtCore>
#endif /* Include guard QT_CORE_H*/

#include "meta.h"

namespace ns_osm {

class Osm_Object;
class Osm_Node;
class Osm_Way;
class Osm_Relation;

class Osm_Subscriber {
	friend class Osm_Object;
	friend class Osm_Map;
private:
	QList<Osm_Object*>		m_sources;
	Meta					m_meta;
protected:
	ns_osm::Meta			get_meta				() const;
	virtual void			handle_event_update		(Osm_Way& source);
	virtual void			handle_event_update		(Osm_Node& source);
	virtual void			handle_event_update		(Osm_Relation& source);
	virtual void			handle_event_update		(Osm_Object& generic_emitter);
	virtual void			handle_event_delete		(Osm_Node& source);
	virtual void			handle_event_delete		(Osm_Way& source);
	virtual void			handle_event_delete		(Osm_Relation& source);
	virtual void			handle_event_delete		(Osm_Object& generic_emitter);

	void					unsubscribe				();
	void					subscribe				(Osm_Object&);
	                        Osm_Subscriber			();
							Osm_Subscriber			(Osm_Subscriber&) = delete;
	Osm_Subscriber			operator=				(Osm_Subscriber&) = delete;
	virtual					~Osm_Subscriber			();
	void					unsubscribe				(Osm_Object&);
};
}
#endif // OSM_SUBSCRIBER_H

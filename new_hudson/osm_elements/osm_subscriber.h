#ifndef OSM_SUBSCRIBER_H
#define OSM_SUBSCRIBER_H

#ifndef QT_CORE_H
#define QT_CORE_H
#include <QtCore>
#endif /* Include guard QT_CORE_H*/

namespace ns_osm {

class Osm_Object;
class Osm_Node;
class Osm_Way;
class Osm_Relation;

class Osm_Subscriber {
private:
	QList<Osm_Object*>	m_sources;
protected:
	void				unsubscribe			();
	void				subscribe			(Osm_Object&);
public:
	void				unsubscribe			(Osm_Object&);
	//void				remove_source		(const Osm_Object&);
	virtual void		handle_event_update	(Osm_Way& source);
	virtual void		handle_event_update	(Osm_Node& source);
	virtual void		handle_event_update	(Osm_Relation& source);
	virtual void		handle_event_update	(Osm_Object& source);
	virtual void		handle_event_delete	(Osm_Node& source);
	virtual void		handle_event_delete	(Osm_Way& source);
	virtual void		handle_event_delete	(Osm_Relation& source);
	virtual void		handle_event_delete	(Osm_Object& source);
	                    Osm_Subscriber		();
	                    Osm_Subscriber		(Osm_Subscriber&) = delete;
	Osm_Subscriber		operator=			(Osm_Subscriber&) = delete;
	virtual				~Osm_Subscriber		();
};
}
#endif // OSM_SUBSCRIBER_H

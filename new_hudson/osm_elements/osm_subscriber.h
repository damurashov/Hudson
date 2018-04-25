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
public:
	enum Meta {
		NONE = 0,
		NODE_ADDED,
		NODE_UPDATED,
		NODE_DELETED,
		WAY_ADDED,
		WAY_UPDATED,
		WAY_DELETED,
		RELATION_ADDED,
		RELATION_UPDATED,
		RELATION_DELETED
	};
private:
	static bool				sf_delegated;
	QList<Osm_Object*>		m_sources;
	Osm_Subscriber::Meta	m_meta;
protected:
	void					stop_broadcast			();
	void					unsubscribe				();
	void					subscribe				(Osm_Object&);
	Osm_Subscriber::Meta	get_meta				() const;
public:
	static bool				is_broadcast_delegated	();
	void					set_meta				(Meta);
	void					unsubscribe				(Osm_Object&);
	virtual void			handle_event_update		(Osm_Way& source);
	virtual void			handle_event_update		(Osm_Node& source);
	virtual void			handle_event_update		(Osm_Relation& source);
	virtual void			handle_event_update		(Osm_Object& source);
	virtual void			handle_event_delete		(Osm_Node& source);
	virtual void			handle_event_delete		(Osm_Way& source);
	virtual void			handle_event_delete		(Osm_Relation& source);
	virtual void			handle_event_delete		(Osm_Object& source);
	                        Osm_Subscriber			();
							Osm_Subscriber			(Osm_Subscriber&) = delete;
	Osm_Subscriber			operator=				(Osm_Subscriber&) = delete;
	virtual					~Osm_Subscriber			();
};
}
#endif // OSM_SUBSCRIBER_H

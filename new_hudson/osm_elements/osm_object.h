#ifndef OSM_OBJECT_H
#define OSM_OBJECT_H

#ifndef QT_CORE_H
#define QT_CORE_H
#include <QtCore>
#endif // include guard QT_CORE_H

#include "osm_subscriber.h"
#include "meta.h"

namespace ns_osm {

class Osm_Object {
protected:
	enum class Type;
	friend class Osm_Map;
private:
	static QHash<long long, bool>	s_id_to_lifestage; /* true - exists, false - destructed */
	const long long					INNER_ID;
	const Type						TYPE;
	static long long				s_inner_id_bound;
	QList<Osm_Subscriber*>			m_subscribers;
	QList<Osm_Subscriber*>			m_active_stack;
	int								mn_subscribers;
	int								mn_osm_object_subscribers;
	bool							f_is_valid;

	bool							is_osm_object			(Osm_Subscriber*) const;
//	                                Osm_Object				() = delete;
protected:
	enum class Type {NODE, WAY, RELATION, GENERIC_EMITTER};

	const Type						get_type				() const;
	long long						get_inner_id			() const;
	void							set_valid				(bool f_valid);
	static bool						is_locked				(long long);
	void							emit_delete				(Meta meta = ns_osm::NONE);
	void							emit_update				(Meta meta = ns_osm::NONE);
	                                Osm_Object				(const Type type = Type::GENERIC_EMITTER);
									Osm_Object				(const Osm_Object&) = delete;
	Osm_Object&						operator=				(const Osm_Object&) = delete;
public:
	void							add_subscriber			(Osm_Subscriber&);
	void							remove_subscriber		(Osm_Subscriber&);
	int								count_subscribers		() const;
	int								count_osm_subscribers	() const; /* Genuine OSM objects : nodes, ways, relations. */
	bool							is_valid				() const;
	virtual							~Osm_Object				();
};	// class Osm_Object

}

#endif // include guard OSM_OBJECT_H

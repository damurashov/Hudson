#ifndef META_H
#define META_H

#ifndef QT_CORE_H
#define QT_CORE_H
#include <QtCore>
#endif /* Include guard QT_CORE_H */

namespace ns_osm {

/*================================================================*/
/*               Concrete events and event groups                 */
/*================================================================*/

enum Event {
	NONE				= 0,
	// Special events go here
	RELATION_MEMBER_ROLE_SET,
	//-----------------------
	NODE_ADDED			= 100,
	NODE_ADDED_FRONT,
	NODE_ADDED_BACK,
	NODE_ADDED_AFTER,
	NODE_UPDATED		= 200,
	NODE_DELETED		= 300,
	NODE_DELETED_FRONT,
	NODE_DELETED_BACK,
	NODE_DELETED_AFTER,
	WAY_ADDED			= 400,
	WAY_UPDATED			= 500,
	WAY_DELETED			= 600,
	RELATION_ADDED		= 700,
	RELATION_UPDATED	= 800,
	RELATION_DELETED	= 900,
	MAP_EVENT			= 1000,
	MAP_DELETED,
	MAP_NODE_ADDED,
	MAP_WAY_ADDED,
	MAP_RELATION_ADDED
	//MAP_SCENE_SHRINKED
};

/*================================================================*/
/*                          Class Meta                            */
/*================================================================*/

class Osm_Object;

class Meta {
public:
	enum Subject {
		SUBJECT_PRIMARY,
		SUBJECT_BEFORE,
		SUBJECT_AFTER
	};
private:
	struct Subj;
	QMap<Subject, Subj>			m_additional_subjects;
	Event						m_event;
	Osm_Object*					mp_primary_subject;
public:
	bool						is_generic_event() const; /* As opposed to a concrete event */
	Meta&						set_event		(Event);
	Meta&						set_pos			(unsigned short subject_position, Subject);
	Meta&						set_subject		(Osm_Object& subject, Subject = SUBJECT_PRIMARY);
	Osm_Object*					get_subject		(Subject = SUBJECT_PRIMARY) const;
	int							get_pos			(Subject) const;
	Event						get_event		() const;
	                            Meta			();
								Meta			(Event);
								Meta			(const Meta&);
								Meta			(Meta&&);
	Meta&						operator=		(const Meta&);
	Meta&						operator=		(Meta&&);
	Meta&						operator=		(Event);
	Meta&						operator=		(Osm_Object& subject);
	bool						operator==		(Event);
	bool						operator==		(const Meta&);
	                            operator int	() const;
};

/*================================================================*/
/*                          Meta::Subj                            */
/*================================================================*/

struct Meta::Subj {
	Osm_Object* p_object;
	int			object_pos;
	            Subj();
};

} /* namespace */

#endif // META_H

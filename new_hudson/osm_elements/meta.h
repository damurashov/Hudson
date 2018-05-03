#ifndef META_H
#define META_H

namespace ns_osm {

/*================================================================*/
/*               Concrete events and event groups                 */
/*================================================================*/

enum Event {
	NONE				= 0,
	// Special events go here
	NODE_ADDED			= 100,
	NODE_ADDED_FRONT,
	NODE_ADDED_BACK,
	NODE_UPDATED		= 200,
	NODE_DELETED		= 300,
	NODE_DELETED_FRONT,
	NODE_DELETED_BACK,
	WAY_ADDED			= 400,
	WAY_UPDATED			= 500,
	WAY_DELETED			= 600,
	RELATION_ADDED		= 700,
	RELATION_UPDATED	= 800,
	RELATION_DELETED	= 900
};

/*================================================================*/
/*                          Class Meta                            */
/*================================================================*/

class Osm_Object;

class Meta {
private:
	Event		m_event;
	Osm_Object* mp_subject;
public:
	Meta&		set_event	(Event);
	Meta&		set_subject	(Osm_Object&);
	Osm_Object*	get_subject	() const;
	Event		get_event	() const;
	            Meta		();
				Meta		(Event);
				Meta		(const Meta&);
				Meta		(Meta&&);
	Meta&		operator=	(const Meta&);
	Meta&		operator=	(Meta&&);
	Meta&		operator=	(Event);
	Meta&		operator=	(Osm_Object& subject);
	bool		operator==	(Event);
	bool		operator==	(const Meta&);
	            operator int() const;
};

} /* namespace */

#endif // META_H

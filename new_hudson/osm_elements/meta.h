#ifndef META_H
#define META_H

namespace ns_osm {

/*================================================================*/
/*               Concrete events and event groups                 */
/*================================================================*/

enum Event {
	NONE				= 0,
	// Special events goes here
	NODE_ADDED			= 10,
	NODE_ADDED_FRONT,
	NODE_ADDED_BACK,
	NODE_UPDATED		= 20,
	NODE_DELETED		= 30,
	NODE_DELETED_FRONT,
	NODE_DELETED_BACK,
	WAY_ADDED			= 40,
	WAY_UPDATED			= 50,
	WAY_DELETED			= 60,
	RELATION_ADDED		= 70,
	RELATION_UPDATED	= 80,
	RELATION_DELETED	= 90
};

/*================================================================*/
/*                          Class Meta                            */
/*================================================================*/

class Meta {
private:
	Event	m_event;
public:
	        Meta		();
			Meta		(Event);
			Meta		(const Meta&);
			Meta		(Meta&&);
	Meta&	operator=	(const Meta&);
	Meta&	operator=	(Meta&&);
	Meta&	operator=	(Event);
	bool	operator==	(Event);
	bool	operator==	(const Meta&);
	        operator int() const;
};

} /* namespace */

#endif // META_H

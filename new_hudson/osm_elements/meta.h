#ifndef META_H
#define META_H

namespace ns_osm {

class Meta {
public:
	enum Event {
		NONE				= 0,
		NODE_ADDED			= 10,
		NODE_ADDED_FRONT,
		NODE_ADDED_BACK,
		NODE_UPDATED		= 20,
		NODE_DELETED		= 30,
		NODE_DELETED_FRONT,
		NODE_DELETED_BACK,
		NODE_DELETED,
		WAY_ADDED			= 40,
		WAY_UPDATED			= 50,
		WAY_DELETED			= 60,
		RELATION_ADDED		= 70,
		RELATION_UPDATED	= 80,
		RELATION_DELETED	= 90
	};
private:
	Event		m_event;
public:
	            Meta		();
				Meta		(Event);
				Meta		(const Meta&);
				Meta		(Meta&&);
	Meta&		operator=	(const Meta&);
	Meta&		operator=	(Meta&&);
	Meta&		operator=	(Event);
	void		set_meta	(Event);
	Event		get_meta	() const;
	            operator int();
	friend bool	operator==	(const Meta&, const Meta&);
	friend bool	operator==	(const Meta&, Event);
	friend bool operator==	(Event, const Meta&);
};

} /* namespace */

#endif // META_H

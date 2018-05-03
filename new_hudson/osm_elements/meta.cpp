#include "meta.h"

using namespace ns_osm;

/*================================================================*/
/*           Constructors, destructors, arithm. ops.              */
/*================================================================*/

Meta::Meta() {
	m_event = Meta::Event::NONE;
}

Meta::Meta(Meta::Event event) {
	m_event = event;
}

Meta::Meta(const Meta& meta) {
	m_event = meta.m_event;
}

Meta::Meta(Meta&& meta) {
	m_event = meta.m_event;
}

Meta& Meta::operator=(const Meta& meta) {
	m_event = meta.m_event;
	return *this;
}

Meta& Meta::operator=(Meta&& meta) {
	m_event = meta.m_event;
	return *this;
}

Meta& Meta::operator=(Meta::Event event) {
	m_event = event;
	return *this;
}

/*================================================================*/
/*                        Public methods                          */
/*================================================================*/

#define BOUNDARY(event) if (m_event >= event) return event
Meta::operator int() {
	BOUNDARY(RELATION_DELETED);
	BOUNDARY(RELATION_UPDATED);
	BOUNDARY(RELATION_ADDED);
	BOUNDARY(WAY_DELETED);
	BOUNDARY(WAY_UPDATED);
	BOUNDARY(WAY_ADDED);
	BOUNDARY(NODE_DELETED);
	BOUNDARY(NODE_UPDATED);
	BOUNDARY(NODE_ADDED);
	return NONE;
}
#undef BOUNDARY

void Meta::set_meta(Event event) {
	m_event = event;
}

void Meta::get_meta() const {
	return m_event;
}

/*================================================================*/
/*                           Friends                              */
/*================================================================*/

bool operator==(const Meta& meta, Meta::Event event) {
	/* When event desribes a group of events */
	if (static_cast<int>(Meta(event)) == event) {
		return event == static_cast<int>(meta);
	/* When event describes a concrete event */
	} else {
		return meta.m_event == event;
	}
}

bool operator==(Meta::Event event, const Meta& meta) {
	return meta == event;
}

bool operator==(const Meta& lhs, const Meta& rhs) {
	/* If both lhs and rhs describe concrete events, their 'm_event' members must be strictly equal.
	   Otherwise to be in a mutual group is enough for equality. */
	return (lhs.m_event == rhs || lhs == rhs.m_event);
}

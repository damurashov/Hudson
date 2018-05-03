#include "meta.h"

using namespace ns_osm;

/*================================================================*/
/*           Constructors, destructors, arithm. ops.              */
/*================================================================*/

Meta::Meta() {
	m_event = ns_osm::Event::NONE;
}

Meta::Meta(ns_osm::Event event) {
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

Meta& Meta::operator=(ns_osm::Event event) {
	m_event = event;
	return *this;
}

bool Meta::operator==(Event event) {
	/* When event describes a group of events */
	if (static_cast<int>(Meta(event)) == event) {
		return event == static_cast<int>(*this);
	/* When event describes a concrete event */
	} else {
		return m_event == event;
	}
}

bool Meta::operator==(const Meta& meta) {
	return (meta == m_event || *this == meta.m_event);
}

/*================================================================*/
/*                        Public methods                          */
/*================================================================*/

#define BOUNDARY(event) if (m_event >= event) return event
Meta::operator int() const {
	BOUNDARY(RELATION_DELETED);
	BOUNDARY(RELATION_UPDATED);
	BOUNDARY(RELATION_ADDED);
	BOUNDARY(WAY_DELETED);
	BOUNDARY(WAY_UPDATED);
	BOUNDARY(WAY_ADDED);
	BOUNDARY(NODE_DELETED);
	BOUNDARY(NODE_UPDATED);
	BOUNDARY(NODE_ADDED);
	return m_event;
}
#undef BOUNDARY

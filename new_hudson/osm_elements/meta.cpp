#include "meta.h"
#include "osm_object.h"

using namespace ns_osm;

/*================================================================*/
/*           Constructors, destructors, arithm. ops.              */
/*================================================================*/

Meta::Meta() {
	m_event = ns_osm::Event::NONE;
	mp_subject = nullptr;
}

Meta::Meta(ns_osm::Event event) {
	m_event = event;
	mp_subject = nullptr;
}

Meta::Meta(const Meta& meta) {
	m_event = meta.m_event;
	mp_subject = meta.mp_subject;
}

Meta::Meta(Meta&& meta) {
	m_event = meta.m_event;
	mp_subject = meta.mp_subject;
}

Meta& Meta::operator=(const Meta& meta) {
	m_event = meta.m_event;
	mp_subject = meta.mp_subject;
	return *this;
}

Meta& Meta::operator=(Meta&& meta) {
	m_event = meta.m_event;
	mp_subject = meta.mp_subject;
	return *this;
}

Meta& Meta::operator=(ns_osm::Event event) {
	m_event = event;
	return *this;
}

Meta& Meta::operator=(Osm_Object& subject) {
	mp_subject = &subject;
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

/*================================================================*/
/*                        Public methods                          */
/*================================================================*/

Meta& Meta::set_event(Event event) {
	m_event = event;
	return *this;
}

Meta& Meta::set_subject(Osm_Object& subject) {
	mp_subject = &subject;
	return *this;
}

Event Meta::get_event() const {
	return m_event;
}

Osm_Object* Meta::get_subject() const {
	return mp_subject;
}

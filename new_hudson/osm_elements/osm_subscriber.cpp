#include "osm_subscriber.h"
#include "osm_object.h"
#include "osm_node.h"
#include "osm_way.h"
#include "osm_relation.h"

using namespace ns_osm;

/*================================================================*/
/*                  Constructors, destructors                     */
/*================================================================*/

Osm_Subscriber::Osm_Subscriber() {}

Osm_Subscriber::~Osm_Subscriber() {
	unsubscribe();
}

/*================================================================*/
/*                      Protected methods                         */
/*================================================================*/

void Osm_Subscriber::unsubscribe(Osm_Object& source) {
	source.remove_subscriber(*this);
	m_sources.removeOne(&source);
}

void Osm_Subscriber::subscribe(Osm_Object& object) {
	object.add_subscriber(*this);
	m_sources.push_back(&object);
}

void Osm_Subscriber::unsubscribe() {
	for (auto it = m_sources.begin(); it != m_sources.end(); ++it) {
		(*it)->remove_subscriber(*this);
	}
	m_sources.clear();
}

Meta Osm_Subscriber::get_meta() const {
	return m_meta;
}

void Osm_Subscriber::handle_event_update (Osm_Node&) {}

void Osm_Subscriber::handle_event_update (Osm_Way&) {}

void Osm_Subscriber::handle_event_update (Osm_Relation&) {}

//void Osm_Subscriber::handle_event_update(Osm_Object &) {}

void Osm_Subscriber::handle_event_delete (Osm_Node&) {}

void Osm_Subscriber::handle_event_delete (Osm_Way&) {}

void Osm_Subscriber::handle_event_delete (Osm_Relation&) {}

//void Osm_Subscriber::handle_event_delete(Osm_Object &) {}

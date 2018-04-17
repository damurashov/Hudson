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
}

void Osm_Subscriber::subscribe(Osm_Object& object) {
	object.add_subscriber(*this);
	m_sources.push_back(&object);
}

/*================================================================*/
/*                        Public methods                          */
/*================================================================*/

void Osm_Subscriber::unsubscribe() {
	for (auto it = m_sources.begin(); it != m_sources.end(); ++it) {
		(*it)->remove_subscriber(*this);
	}
}

//void Osm_Subscriber::remove_source(const Osm_Object& source) {
//	m_sources.removeAll(&source);
//}

void Osm_Subscriber::handle_event_update (Osm_Node& source) {}

void Osm_Subscriber::handle_event_update (Osm_Way& source) {}

void Osm_Subscriber::handle_event_update (Osm_Relation& source) {}

void Osm_Subscriber::handle_event_update(Osm_Object &source) {}

void Osm_Subscriber::handle_event_delete (Osm_Node& source) {}

void Osm_Subscriber::handle_event_delete (Osm_Way& source) {}

void Osm_Subscriber::handle_event_delete (Osm_Relation& source) {}

void Osm_Subscriber::handle_event_delete(Osm_Object &source) {}

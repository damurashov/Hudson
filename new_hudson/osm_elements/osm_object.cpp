#include "osm_object.h"
#include "osm_node.h"
#include "osm_way.h"
#include "osm_relation.h"

using namespace ns_osm;

/*================================================================*/
/*                        Static members                          */
/*================================================================*/

long long						Osm_Object::s_osm_id_bound(-1);
long long						Osm_Object::s_inner_id_bound(0xFFFFFFFFFFFFFFFF);

/*================================================================*/
/*                  Constructors, destructors                     */
/*================================================================*/

Osm_Object::Osm_Object() :
    OSM_ID(s_osm_id_bound--),
	INNER_ID(s_inner_id_bound++),
	TYPE(Osm_Object::Type::NODE)
{
	f_is_valid = true;
	m_attrmap[QString("id")] = QString::number(OSM_ID);
	//reg_osm_object(this);
}

Osm_Object::Osm_Object(const Osm_Object::Type type) :
	OSM_ID(s_osm_id_bound--),
	INNER_ID(s_inner_id_bound++),
	TYPE(type)
{
	f_is_valid = true;
	m_attrmap[QString("id")] = QString::number(OSM_ID);
	//reg_osm_object(this);
}


Osm_Object::Osm_Object(const QString &id, const Osm_Object::Type type):
    OSM_ID(id.toLongLong()),
	INNER_ID(s_inner_id_bound++),
	TYPE(type)
{
	f_is_valid = true;
	m_attrmap[QString("id")] = QString::number(OSM_ID);
	if (s_osm_id_bound >= OSM_ID) {
		s_osm_id_bound = (OSM_ID - 1);
	}
}

Osm_Object::~Osm_Object() {
}

/*================================================================*/
/*                      Protected methods                         */
/*================================================================*/

const Osm_Object::Type Osm_Object::get_type() const {
	return TYPE;
}

void Osm_Object::set_valid(bool f_valid) {
	f_is_valid = f_valid;
}


long long Osm_Object::get_inner_id() const {
	return INNER_ID;
}

void Osm_Object::emit_delete() {
	for (auto it = m_subscribers.begin(); it != m_subscribers.end(); ++it) {
		it->remove_source(*this);
		switch (get_type()) {
		case Type::NODE:
			it->handle_event_delete(*static_cast<Osm_Node*>(this));
			break;
		case Type::WAY:
			it->handle_event_delete(*static_cast<Osm_Way*>(this));
			break;
		case Type::RELATION:
			it->handle_event_delete(*static_cast<Osm_Relation*>(this));
			break;
		}
		it->handle_event_delete(*static_cast<Osm_Object*>(this));
	}
}

void Osm_Object::emit_update() {
	for (auto it = m_subscribers.begin(); it != m_subscribers.end(); ++it) {
		switch (get_type()) {
		case Type::NODE:
			it->handle_event_update(*static_cast<Osm_Node*>(this));
			break;
		case Type::WAY:
			it->handle_event_update(*static_cast<Osm_Way*>(this));
			break;
		case Type::RELATION:
			it->handle_event_update(*static_cast<Osm_Relation*>(this));
			break;
		}
		it->handle_event_update(*static_cast<Osm_Object*>(this));
	}
}

/*================================================================*/
/*                        Public methods                          */
/*================================================================*/

void Osm_Object::add_subscriber(Osm_Subscriber& subscriber) {
	if (m_subscribers.indexOf(&subscriber) == -1) {
		m_subscribers.push_back(&subscriber);
	}
}

void Osm_Object::remove_subscriber(Osm_Subscriber& subscriber) {
	m_subscribers.removeAll(&subscriber);
}

QString Osm_Object::get_attr_value(const QString& key) const {
	return m_attrmap[key];
}

QString Osm_Object::get_tag_value(const QString &key) const {
	return m_tagmap[key];
}

const QMap<QString, QString>& Osm_Object::get_tag_map() const {
	return m_tagmap;
}

const QMap<QString, QString>& Osm_Object::get_attr_map() const {
	return m_attrmap;
}

long long Osm_Object::get_id() const {
	return OSM_ID;
}

void Osm_Object::set_tag(const QString &key, const QString &value) {
	m_tagmap[key] = value;
}

void Osm_Object::set_attr(const QString &key, const QString &value) {
	if (key == "id")
		return;
	m_attrmap[key] = value;
}

void Osm_Object::remove_tag(const QString &key) {
	m_tagmap.remove(key);
}


void Osm_Object::clear_tags() {
	m_tagmap.clear();
}

bool Osm_Object::is_valid() const {
	return f_is_valid;
}

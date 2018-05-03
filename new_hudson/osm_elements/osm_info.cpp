#include "osm_info.h"

using namespace ns_osm;

/*================================================================*/
/*                        Static members                          */
/*================================================================*/

long long Osm_Info::s_osm_id_bound = -1;

/*================================================================*/
/*          Constructors, destructors, arithmetic ops.            */
/*================================================================*/

Osm_Info::Osm_Info() : OSM_ID(s_osm_id_bound--) {
	m_attrmap[QString("id")] = QString::number(OSM_ID);
}

Osm_Info::Osm_Info(const QString& id) : OSM_ID(id.toLongLong()) {
	m_attrmap[QString("id")] = QString::number(OSM_ID);
	if (s_osm_id_bound >= OSM_ID) {
		s_osm_id_bound = OSM_ID - 1;
	}
}

Osm_Info::Osm_Info(long long id) : OSM_ID(id) {
	m_attrmap[QString("id")] = QString::number(OSM_ID);
	if (s_osm_id_bound >= OSM_ID) {
		s_osm_id_bound = OSM_ID - 1;
	}
}

Osm_Info::Osm_Info(const Osm_Info& info) : OSM_ID(s_osm_id_bound--) {
	m_tagmap = info.m_tagmap;
	m_attrmap = info.m_attrmap;
}

Osm_Info& Osm_Info::operator=(const Osm_Info& info) {
	m_tagmap = info.m_tagmap;
	m_attrmap = info.m_attrmap;
	return *this;
}

/*================================================================*/
/*                        Public methods                          */
/*================================================================*/

QString Osm_Info::get_attr_value(const QString& key) const {
	return m_attrmap[key];
}

QString Osm_Info::get_tag_value(const QString &key) const {
	return m_tagmap[key];
}

const QMap<QString, QString>& Osm_Info::get_tag_map() const {
	return m_tagmap;
}

const QMap<QString, QString>& Osm_Info::get_attr_map() const {
	return m_attrmap;
}

long long Osm_Info::get_id() const {
	return OSM_ID;
}

void Osm_Info::set_tag(const QString &key, const QString &value) {
	m_tagmap[key] = value;
}

void Osm_Info::set_attr(const QString &key, const QString &value) {
	if (key == "id")
		return;
	m_attrmap[key] = value;
}

void Osm_Info::remove_tag(const QString &key) {
	m_tagmap.remove(key);
}

void Osm_Info::clear_tags() {
	m_tagmap.clear();
}

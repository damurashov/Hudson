#include "osm_node.h"
using namespace ns_osm;
// ===================== Constructors, destructors =====================
Osm_Node::Osm_Node() : Osm_Object(Osm_Object::Type::NODE) {
}

Osm_Node::Osm_Node(const QString &id,
                   const QString &latitude,
                   const QString &longitude):
	Osm_Object(id, Osm_Object::Type::NODE)
{
	set_attr("lat", latitude);
	set_attr("lon", longitude);
	m_lat = latitude.toDouble();
	m_lon = longitude.toDouble();
}

Osm_Node::Osm_Node(const QString &latitude, const QString &longitude):
	Osm_Object(Osm_Object::Type::NODE)
{
	//reg_osm_object(this);
	set_attr("lat", latitude);
	set_attr("lon", longitude);
	m_lat = latitude.toDouble();
	m_lon = longitude.toDouble();
}

Osm_Node::Osm_Node(const double& latitude, const double& longitude) :
	Osm_Object(Osm_Object::Type::NODE)
{
	//reg_osm_object(this);
	m_lat = latitude;
	m_lon = longitude;
	set_attr("lat", QString(QString::number(m_lat)));
	set_attr("lon", QString(QString::number(m_lon)));
}

Osm_Node::~Osm_Node() {
	//unreg_osm_object(this);
}

// ========================= Private methods =========================
void Osm_Node::handle_child_del(Osm_Object* child) {
}

// =========================== Public methods ===========================

double Osm_Node::get_lat() const {
	return m_lat;
}

double Osm_Node::get_lon() const {
	return m_lon;
}

void Osm_Node::set_lat(const double &latitude) {
	m_lat = latitude;
	set_attr(QString("lat"), QString(QString::number(m_lat)));
}

void Osm_Node::set_lon(const double &longitude) {
	m_lon = longitude;
	set_attr(QString("lon"), QString(QString::number(m_lon)));
}



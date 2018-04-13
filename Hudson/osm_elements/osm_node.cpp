#include "osm_node.h"

// ===================== Constructors, destructors =====================
Osm_Node::Osm_Node() : Osm_Object() {
}

Osm_Node::Osm_Node(const QString &id,
                   const QString &latitude,
                   const QString &longitude):
    Osm_Object(id)
{
	set_attr("lat", latitude);
	set_attr("lon", longitude);
	m_lat = latitude.toDouble();
	m_lon = longitude.toDouble();
}

Osm_Node::Osm_Node(const QString &latitude, const QString &longitude):
    Osm_Object()
{
	set_attr("lat", latitude);
	set_attr("lon", longitude);
	m_lat = latitude.toDouble();
	m_lon = longitude.toDouble();
}

Osm_Node::Osm_Node(const double& latitude, const double& longitude) :
    Osm_Object()
{
	m_lat = latitude;
	m_lon = longitude;
	set_attr("lat", QString(m_lat));
	set_attr("lon", QString(m_lon));
}

Osm_Node::~Osm_Node() {
}

// ========================= Private methods =========================
void Osm_Node::handle_child_del(Osm_Object* child) {
}

// ========================= Protected methods =========================
const Osm_Object::Type Osm_Node::get_type() const {
	return Osm_Object::Type::NODE;
}


// =========================== Public methods ===========================
Osm_Node* Osm_Node::get_by_id(const long long &id) const {
	return static_cast<Osm_Node*>(get_obj_by_id(Osm_Object::Type::NODE, id));
}

bool Osm_Node::is_valid() const {
	return true;
}

double Osm_Node::get_lat() const {
	return m_lat;
}

double Osm_Node::get_lon() const {
	return m_lon;
}

void Osm_Node::set_lat(const double &latitude) {
	m_lat = latitude;
	set_attr(QString("lat"), QString(m_lat));
}

void Osm_Node::set_lon(const double &longitude) {
	m_lon = longitude;
	set_attr(QString("lon"), QString(m_lon));
}



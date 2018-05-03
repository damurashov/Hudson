#include "osm_node.h"
using namespace ns_osm;

/*================================================================*/
/*                  Constructors, destructors                     */
/*================================================================*/

Osm_Node::Osm_Node(const QString &id,
                   const QString &latitude,
                   const QString &longitude):
    Osm_Object(Osm_Object::Type::NODE),
    Osm_Info(id)
{
	set_attr("lat", latitude);
	set_attr("lon", longitude);
	m_lat = latitude.toDouble();
	m_lon = longitude.toDouble();
	correct();
}

Osm_Node::Osm_Node(const QString &latitude, const QString &longitude):
	Osm_Object(Osm_Object::Type::NODE)
{
	set_attr("lat", latitude);
	set_attr("lon", longitude);
	m_lat = latitude.toDouble();
	m_lon = longitude.toDouble();
	correct();
}

Osm_Node::Osm_Node(const double& latitude, const double& longitude) :
	Osm_Object(Osm_Object::Type::NODE)
{
	m_lat = latitude;
	m_lon = longitude;
	set_attr("lat", QString(QString::number(m_lat)));
	set_attr("lon", QString(QString::number(m_lon)));
	correct();
}

Osm_Node::~Osm_Node() {
	emit_delete();
}

/*================================================================*/
/*                       Private methods                          */
/*================================================================*/

void Osm_Node::correct() {
	while (m_lon > 180.0) {
		m_lon -= 360.0;
	}
	while (m_lon <= -180.0) {
		m_lon += 360.0;
	}
	if (m_lat > 90 || m_lat < -90) {
		set_valid(false);
	}
}

/*================================================================*/
/*                        Public methods                          */
/*================================================================*/

double Osm_Node::get_lat() const {
	return m_lat;
}

double Osm_Node::get_lon() const {
	return m_lon;
}

void Osm_Node::set_lat(const double &latitude) {
	m_lat = latitude;
	set_attr(QString("lat"), QString(QString::number(m_lat)));
	emit_update();
}

void Osm_Node::set_lon(const double &longitude) {
	m_lon = longitude;
	correct();
	set_attr(QString("lon"), QString(QString::number(m_lon)));
	emit_update();
}

void Osm_Node::set_lat_lon(const double &latitude, const double &longitude) {
	m_lat = latitude;
	m_lon = longitude;
	correct();
	set_attr(QString("lat"), QString(QString::number(m_lat)));
	set_attr(QString("lon"), QString(QString::number(m_lon)));
	emit_update();
}

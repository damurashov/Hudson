#include "coord_handler.h"

using namespace ns_osm;

/*================================================================*/
/*                        Static members                          */
/*================================================================*/

//const double Coord_Handler::ONE_METER = 0.0000036;
const double Coord_Handler::ONE_METER = 8.98 / 1000000;
const double Coord_Handler::EARTH_RADIUS = 6378137;
const double Coord_Handler::PRESCALER = 100000;

/*================================================================*/
/*                  Constructors, destructors                     */
/*================================================================*/

Coord_Handler::Coord_Handler() {
	m_start = QPointF(20000, 20000);
	m_scale = 0.0;
	mp_map = nullptr;
}

Coord_Handler::Coord_Handler(const Coord_Handler& ch) : mp_map(ch.mp_map) {
	m_start = ch.m_start;
	m_scale = ch.m_scale;
}

/*================================================================*/
/*                       Private methods                          */
/*================================================================*/

double Coord_Handler::y2lat(double y) const {
	return rad2deg(atan(exp(deg2rad(y))) * 2 - M_PI/2);
}

double Coord_Handler::x2lon(double x) const {
	return x;
}

double Coord_Handler::lat2y(double lat) const {
	return (rad2deg(log(tan(deg2rad(lat)/2+M_PI/4))));
}

double Coord_Handler::lon2x(double lon) const {
	return lon;
}

double Coord_Handler::deg2rad(double deg) const {
	return deg / (180.0 / M_PI);
}

double Coord_Handler::rad2deg(double rad) const {
	return rad * 180 / M_PI;
}

/*================================================================*/
/*                      Protected methods                         */
/*================================================================*/

void Coord_Handler::handle_event_delete(Osm_Object&) {
	mp_map = nullptr;
}

/*================================================================*/
/*                        Public methods                          */
/*================================================================*/

bool Coord_Handler::is_ready() const {
	if (m_start.x() == 20000 || m_start.y() == 20000) {
		return false;
	}
	if (m_scale == 0.0) {
		return false;
	}
	if (mp_map == nullptr) {
		return false;
	}
	return true;
}

double Coord_Handler::get_scale() const {
	return m_scale;
}

void Coord_Handler::set_map(Osm_Map& map) {
	mp_map = &map;
	subscribe(map);
}

void Coord_Handler::set_scale(double scale) {
	m_scale = scale;
}

void Coord_Handler::set_start_point(Osm_Node& node) {
	set_start_point(QPointF(node.get_lon(), node.get_lat()));
}

void Coord_Handler::set_start_point(QPointF geopoint) {
	QPointF point(mp_map->get_scene_coord(geopoint));
	point.setX(point.x() / ONE_METER / m_scale);
	point.setY(point.y() / ONE_METER / m_scale);
	m_start = point;
}

QPointF Coord_Handler::get_pos_on_scene(Osm_Node& node) const {
	QPointF point(mp_map->get_scene_coord(&node));
	point.setX(lon2x(point.x()) * PRESCALER);
	point.setY(lat2y(point.y()) * (-PRESCALER));
//	point = point - m_start;
//	point.setY(-point.y());
	return point;
}

QPointF Coord_Handler::get_geo_coords(QPointF point /* scene point */) const {
	point.setY(-point.y() / PRESCALER);
	point.setX(point.x() / PRESCALER);
//	point = point + m_start;
	point.setX(x2lon(point.x()));
	point.setY(y2lat(point.y()));
	return mp_map->get_geo_coord(point);
}

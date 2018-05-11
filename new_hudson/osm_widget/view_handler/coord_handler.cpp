#include "coord_handler.h"

using namespace ns_osm;

/*================================================================*/
/*                        Static members                          */
/*================================================================*/

//const double Coord_Handler::ONE_METER = 0.0000036;
const double Coord_Handler::ONE_METER = 8.98 / 1000000;

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
	point.setX(point.x() / ONE_METER / m_scale);
	point.setY(point.y() / ONE_METER / m_scale);
	point = point - m_start;
	point.setY(-point.y());
	return point;
}

QPointF Coord_Handler::get_geo_coords(QPointF point /* scene point */) const {
	point.setY(-point.y());
	point = point + m_start;
	point.setX(point.x() * ONE_METER * m_scale);
	point.setY(point.y() * ONE_METER * m_scale);
	return mp_map->get_geo_coord(point);
}

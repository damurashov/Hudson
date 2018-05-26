#include "coord_handler.h"

using namespace ns_osm;

/*================================================================*/
/*                        Static members                          */
/*================================================================*/

const double Coord_Handler::EARTH_RADIUS = 6378137;
const double Coord_Handler::PRESCALER = 100000;

/*================================================================*/
/*                  Constructors, destructors                     */
/*================================================================*/

Coord_Handler::Coord_Handler() {
	mp_map = nullptr;
	reset_autorects();
}

Coord_Handler::Coord_Handler(const Coord_Handler& ch) : mp_map(ch.mp_map) {
}

/*================================================================*/
/*                       Private methods                          */
/*================================================================*/

QRectF Coord_Handler::get_normalized_rect() const {
	QRectF rect;

	if (!is_map_set()) {
		return rect;
	}
	if (is_valid_bound(mp_map->get_bound())) {
		rect = mp_map->get_bound();
	} else {
		double width_normal = m_autorect_normal.right() - m_autorect_normal.left();
		double width_180 = m_autorect_180.left() - m_autorect_180.right();
		rect = (width_180 < width_normal ? m_autorect_180 : m_autorect_normal);
	}
	if (has_issue_180(rect)) {
		rect.setRight(rect.right() + 360.0);
	}

	return rect;
}

void Coord_Handler::reset_autorects() {
	const double BIG = 1000.0;

	m_autorect_normal.setLeft(BIG);
	m_autorect_normal.setRight(-BIG);
	m_autorect_normal.setTop(-BIG);
	m_autorect_normal.setBottom(BIG);
	m_autorect_180.setLeft(BIG);
	m_autorect_180.setRight(-BIG);
	m_autorect_180.setTop(-BIG);
	m_autorect_180.setBottom(BIG);
}

void Coord_Handler::recalculate_autorects() {
	if (!is_map_set()) {
		return;
	}
	reset_autorects();
	for (Osm_Map::cnode_iterator it = mp_map->cnbegin(); it != mp_map->cnend(); ++it) {
		fit_autorects(**it);
	}
}

bool Coord_Handler::is_map_set() const {
	return mp_map != nullptr;
}

bool Coord_Handler::is_valid_bound(const QRectF& rect) const {
	if (rect.width() == 0.0 || rect.height() == 0.0) {
		return false;
	}
	if (std::abs(rect.bottom()) > 90 || std::abs(rect.top()) > 90 || rect.bottom() > rect.top()) {
		return false;
	}
	if (std::abs(rect.left()) >= 180 || std::abs(rect.right()) > 180) {
		return false;
	}
	return true;
}

bool Coord_Handler::has_issue_180(const QRectF& rect) const {
	return rect.right() < rect.left();
}

void Coord_Handler::fit_autorects(const Osm_Node& node)  {
	double lat = node.get_lat();
	double lon = node.get_lon();

	/* Mutual vertical bounds, ... */
	if (lat > m_autorect_normal.top()) {
		m_autorect_normal.setTop(lat);
		m_autorect_180.setTop(lat);
	}
	if (lat < m_autorect_normal.bottom()) {
		m_autorect_normal.setBottom(lat);
		m_autorect_180.setBottom(lat);
	}
	/* ... but different horisontal ones */
	if (lon < m_autorect_normal.left()) {
		m_autorect_normal.setLeft(lon);
	}
	if (lon > m_autorect_normal.right()) {
		m_autorect_normal.setRight(lon);
	}
	if (lon < m_autorect_180.left() && lon > 0) {
		m_autorect_180.setLeft(lon);
	}
	if (lon > m_autorect_180.right() && lon <= 0) {
		m_autorect_180.setRight(lon);
	}
}

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
	if (get_meta().get_event() != MAP_DELETED) {
		return;
	}
	mp_map = nullptr;
	reset_autorects();
}

void Coord_Handler::handle_event_update(Osm_Object&) {
	Event event = get_meta().get_event();

	if (event == MAP_CLEARED) {
		reset_autorects();
	} else if (event == MAP_NODE_UPDATED || event == MAP_NODE_ADDED) {
		if (get_meta().get_subject() == nullptr) {
			return;
		}
		Osm_Node& node = *static_cast<Osm_Node*>(get_meta().get_subject());
		fit_autorects(node);
	}
}

/*================================================================*/
/*                        Public methods                          */
/*================================================================*/

void Coord_Handler::set_map(Osm_Map& map) {
	unsubscribe();
	mp_map = &map;
	subscribe(map);
	recalculate_autorects();
}

QPointF Coord_Handler::get_pos_on_scene(Osm_Node& node) const {
	QPointF point(node.get_lon(), node.get_lat());

	if (get_normalized_rect().right() > 180 && point.x() <= 0) {
		point.setX(point.x() + 360.0);
	}
	point.setX(lon2x(point.x()) * PRESCALER);
	point.setY(lat2y(point.y()) * (-PRESCALER));

	return point;
}

QPointF Coord_Handler::get_geo_coords(QPointF point /* scene point */) const {
	point.setY(point.y() / (-PRESCALER));
	point.setX(point.x() / PRESCALER);
	point.setX(x2lon(point.x()));
	point.setY(y2lat(point.y()));
	if (get_normalized_rect().right() > 180 && point.x() > 180) {
		point.setX(point.x() - 360.0);
	}
	return point;
}

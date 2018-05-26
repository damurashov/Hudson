#ifndef COORD_HANDLER_H
#define COORD_HANDLER_H

#include "osm_elements.h"
#include <math.h>

namespace ns_osm {

class Coord_Handler : public Osm_Subscriber {
	static const double EARTH_RADIUS;
	static const double PRESCALER;
	const Osm_Map*		mp_map;
	bool				f_force_dynamic_bound;
	QRectF				m_autorect_normal;
	QRectF				m_autorect_180;

	QRectF				get_normalized_rect		() const;
	void				reset_autorects			();
	void				recalculate_autorects	();
	bool				is_map_set				() const;
	bool				is_valid_bound			(const QRectF&) const;
	bool				has_issue_180			(const QRectF&) const;
	void				fit_autorects			(const Osm_Node&);
	double				y2lat					(double y) const;
	double				x2lon					(double x) const;
	double				lat2y					(double lat) const;
	double				lon2x					(double lon) const;
	double				deg2rad					(double deg) const;
	double				rad2deg					(double rad) const;
protected:
	void				handle_event_delete		(Osm_Object&) override;
	void				handle_event_update		(Osm_Object&) override;
public:
	void				set_map					(Osm_Map&);
	QPointF				get_pos_on_scene		(Osm_Node&) const;
	QPointF				get_geo_coords			(QPointF scene_pos) const;
	                    Coord_Handler			();
						Coord_Handler			(const Coord_Handler&);
	Coord_Handler		operator=				(const Coord_Handler&) = delete;
};

}
#endif // COORD_HANDLER_H

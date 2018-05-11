#ifndef COORD_HANDLER_H
#define COORD_HANDLER_H

#include "osm_elements.h"

namespace ns_osm {

class Coord_Handler : public Osm_Subscriber {
	static const double ONE_METER;
	const Osm_Map*		mp_map;
	double				m_scale; /* 1 to m_scale */
	QPointF				m_start;
protected:
	void				handle_event_delete	(Osm_Object& override);
public:
	bool				is_ready			() const;
	double				get_scale			() const;
	void				set_map				(Osm_Map&);
	void				set_scale			(double scale);
	void				set_start_point		(Osm_Node&);
	void				set_start_point		(QPointF geopoint);
	QPointF				get_pos_on_scene	(Osm_Node&) const;
	QPointF				get_geo_coords		(QPointF scene_pos) const;
	                    Coord_Handler		();
						Coord_Handler		(const Coord_Handler&);
	Coord_Handler		operator=			(const Coord_Handler&) = delete;
};

}
#endif // COORD_HANDLER_H

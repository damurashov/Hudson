#include "osm_widget.h"
using namespace ns_osm;
// ===================== Constructors, destructors =====================

Osm_Widget::Osm_Widget(QWidget* p_parent) :
    QWidget(p_parent),
    m_xml_handler(m_nodes_hash, m_ways_hash, m_relations_hash, m_bounding_rect) {}

Osm_Widget::~Osm_Widget() {
	remove_all_osm_objects();
}

// ========================== Protected methods =========================

void Osm_Widget::remove_all_osm_objects() {
	QHash<long long, Osm_Node*>::iterator	  it_n;
	QHash<long long, Osm_Way*>::iterator	  it_w;
	QHash<long long, Osm_Relation*>::iterator it_r;

	for (it_n = m_nodes_hash.begin(); it_n != m_nodes_hash.end(); ++it_n) {
		if (it_n.value() != nullptr) {
			delete it_n.value();
		}
	}
	for (it_w = m_ways_hash.begin(); it_w != m_ways_hash.end(); ++it_w) {
		if (it_w.value() != nullptr) {
			delete it_w.value();
		}
	}
	for (it_r = m_relations_hash.begin(); it_r != m_relations_hash.end(); ++it_r) {
		if (it_r.value() != nullptr) {
			delete it_r.value();
		}
	}

	m_nodes_hash.clear();
	m_ways_hash.clear();
	m_relations_hash.clear();
}

void Osm_Widget::compute_bounding_rect() {
	QHash<long long, Osm_Node*>::iterator	it;
	long long								minlat = 90;
	long long 								minlon = 180;
	long long 								maxlat = -90;
	long long 								maxlon = -180;
	long long								temp_lat;
	long long								temp_lon;

	if (!m_nodes_hash.isEmpty()) {
		for (it = m_nodes_hash.begin(); it != m_nodes_hash.end(); ++it) {
			temp_lat = it.value()->get_lat();
			temp_lon = it.value()->get_lon();

			if (temp_lat < minlat) {
				minlat = temp_lat;
			}
			if (temp_lat > maxlat) {
				maxlat = temp_lat;
			}
			if (temp_lon < minlon) {
				minlon = temp_lon;
			}
			if (temp_lon > maxlon) {
				maxlon = temp_lon;
			}
		}
		//m_bounding_rect.setCoords(min_lon, min_lat, max_lon, max_lat);
		m_bounding_rect.setLeft(minlon);
		m_bounding_rect.setBottom(minlat);
		m_bounding_rect.setRight(maxlon);
		m_bounding_rect.setTop(maxlat);
	}
}

// ========================== Public methods =========================

bool Osm_Widget::load_from_xml(const QString &xml_path) {
	bool	     f_done;

	remove_all_osm_objects();
	f_done = m_xml_handler.load_from_xml(xml_path);

	if (f_done) {
		if (m_bounding_rect.height() == 0.0 || m_bounding_rect.width() == 0.0) {
			/* The bounding rect was not parsed successful */
			compute_bounding_rect();
		}
	}
	//reset_view();
	return f_done;
}

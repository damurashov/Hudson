#include "osm_view.h"

using namespace ns_osm;

/*================================================================*/
/*                  Constructors, destructors                     */
/*================================================================*/

Osm_View::~Osm_View() {}

/*================================================================*/
/*                       Private methods                          */
/*================================================================*/

void Osm_View::scale_view(double scale_factor) {
	double factor = transform().scale(scale_factor, scale_factor).mapRect(QRectF(0,0,1,1)).width();
	if (factor < 0.07 || factor > 100.0) {
		return;
	}
	scale(scale_factor, scale_factor);
}

/*================================================================*/
/*                      Protected methods                         */
/*================================================================*/

QPointF Osm_View::get_last_pos() const {
	return m_last_pos;
}

void Osm_View::set_last_pos(const QPointF& point) {
	m_last_pos = point;
}

void Osm_View::wheelEvent(QWheelEvent* p_event) {
	switch (p_event->modifiers()) {
	case (Qt::KeyboardModifier::ControlModifier):
		scale_view(pow(2.0, -(p_event->delta() / 240.0)));
		break;
	case (Qt::KeyboardModifier::ShiftModifier):
		scrollContentsBy(pow(2.0, -(p_event->delta() / 240.0)), 0.0);
		break;
	case (Qt::KeyboardModifier::NoModifier):
		scrollContentsBy(0.0, pow(2.0, -(p_event->delta() / 240.0)));
		break;
	}
}

void Osm_View::mouseReleaseEvent(QMouseEvent* p_event) {
	switch (p_event->button()) {
	case Qt::MouseButton::MiddleButton:
		scrollContentsBy(p_event->pos().x() - get_last_pos().x(), p_event->pos().y() - get_last_pos().y());
		//set_last_pos(p_event->pos());
		break;
	case Qt::MouseButton::RightButton:
		emit signal_blank_area_clicked(p_event->pos(), p_event->button());
		break;
	case Qt::MouseButton::LeftButton:
		emit signal_blank_area_clicked(mapToScene(p_event->pos()), p_event->button());
		break;
	default:
		break;
	}
}

void Osm_View::mouseMoveEvent(QMouseEvent* p_event) {
	if (p_event->button() != Qt::MouseButton::MiddleButton) {
		return;
	}
	scrollContentsBy(p_event->pos().x() - get_last_pos().x(), p_event->pos().y() - get_last_pos().y());
	set_last_pos(p_event->pos());
}

void Osm_View::mousePressEvent(QMouseEvent* p_event) {
	if (p_event->button() == Qt::MouseButton::MiddleButton) {
		set_last_pos(p_event->pos());
	}
}

#include "osm_view.h"

using namespace ns_osm;

/*================================================================*/
/*                  Constructors, destructors                     */
/*================================================================*/

Osm_View::Osm_View() {
//	setDragMode(ScrollHandDrag);
	setRenderHints(QPainter::Antialiasing);
	setRenderHint(QPainter::SmoothPixmapTransform, true);
	setTransformationAnchor(AnchorUnderMouse);
	setViewportUpdateMode(FullViewportUpdate);
//	setMinimumSize(600,400);
//	setViewportUpdateMode(BoundingRectViewportUpdate);
}

Osm_View::~Osm_View() {}

/*================================================================*/
/*                       Private methods                          */
/*================================================================*/

//void Osm_View::scale_view(double scale_factor) {
//	scale(scale_factor, scale_factor);
//}

/*================================================================*/
/*                      Protected methods                         */
/*================================================================*/

//QPointF Osm_View::get_last_pos() const {
//	return m_last_pos;
//}

void Osm_View::set_last_pos(const QPointF& point) {
	m_last_pos = point;
}

void Osm_View::wheelEvent(QWheelEvent* p_event) {
	double factor;
	switch (p_event->modifiers()) {
	case (Qt::KeyboardModifier::NoModifier):
		factor = pow(2.0, (p_event->delta() / 240.0));
//		factor = p_event->delta() / 240.0;
		scale(factor, factor);
		break;
	case (Qt::KeyboardModifier::ShiftModifier):
//		scrollContentsBy(pow(2.0, (p_event->delta() / 20)), 0.0);
		translate(pow(2.0, (p_event->delta() / 20)), 0.0);
		break;
	case (Qt::KeyboardModifier::ControlModifier):
//		scrollContentsBy(0.0, pow(2.0, (p_event->delta() / 20)));
		translate(0.0, pow(2.0, (p_event->delta() / 20)));
		break;
	default:
		break;
	}
}

void Osm_View::mouseReleaseEvent(QMouseEvent* p_event) {
	switch (p_event->button()) {
	case Qt::MouseButton::MiddleButton:
		//translate(p_event->pos().x() - get_last_pos().x(), p_event->pos().y() - get_last_pos().y());
		//set_last_pos(p_event->pos());
		break;
	case Qt::MouseButton::RightButton:
		if (items(p_event->pos()).empty()) {
			emit signal_blank_area_clicked(p_event->pos(), p_event->button());
		}
		break;
	case Qt::MouseButton::LeftButton:
		if (items(p_event->pos()).empty()) {
			emit signal_blank_area_clicked(mapToScene(p_event->pos()), p_event->button());
		}
		break;
	default:
		break;
	}
	QGraphicsView::mouseReleaseEvent(p_event);
}

//void Osm_View::mouseMoveEvent(QMouseEvent* p_event) {
//	if (p_event->button() != Qt::MouseButton::MiddleButton) {
//		return;
//	}
//	translate(p_event->pos().x() - get_last_pos().x(), p_event->pos().y() - get_last_pos().y());
//	set_last_pos(p_event->pos());
//}

//void Osm_View::mousePressEvent(QMouseEvent* p_event) {
//	if (p_event->button() == Qt::MouseButton::MiddleButton) {
//		set_last_pos(p_event->pos());
//	}
//}

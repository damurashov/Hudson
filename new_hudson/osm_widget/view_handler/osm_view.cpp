#include "osm_view.h"

using namespace ns_osm;

/*================================================================*/
/*                  Constructors, destructors                     */
/*================================================================*/

Osm_View::Osm_View(QWidget* p_parent) : QGraphicsView(p_parent) {
//	setDragMode(ScrollHandDrag);
	setRenderHint(QPainter::Antialiasing, true);
	setRenderHint(QPainter::SmoothPixmapTransform, true);
	setTransformationAnchor(AnchorUnderMouse);
	setViewportUpdateMode(FullViewportUpdate);
//	setMinimumSize(600,400);
//	setViewportUpdateMode(BoundingRectViewportUpdate);
}

Osm_View::~Osm_View() {}

/*================================================================*/
/*                      Protected methods                         */
/*================================================================*/

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

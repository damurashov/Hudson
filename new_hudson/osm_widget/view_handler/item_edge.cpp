#include "item_edge.h"

using namespace ns_osm;

/*================================================================*/
/*                  Constructors, destructors                     */
/*================================================================*/

Item_Edge::Item_Edge(const Coord_Handler& handler,
                     Osm_Node& node1,
                     Osm_Node& node2,
                     Osm_Way& way,
                     QGraphicsItem* p_parent)
                     :
                       QGraphicsObject(p_parent),
                       Edge(node1, node2),
                       m_coord_handler(handler),
                       m_way(way)
{
	subscribe(*first());
	subscribe(*second());
	setFlags(ItemIsSelectable);
	setFlag(ItemSendsGeometryChanges);
	setActive(true);
	setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton | Qt::MidButton);
}

Item_Edge::Item_Edge(const Coord_Handler& handler,
                     const Edge& edge,
                     Osm_Way& way,
                     QGraphicsItem *p_parent)
                     :
                       QGraphicsObject(p_parent),
                       Edge(edge),
                       m_coord_handler(handler),
                       m_way(way)
{
	subscribe(*first());
	subscribe(*second());
	setFlags(ItemIsSelectable);
	setFlag(ItemSendsGeometryChanges);
	setActive(true);
	setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton | Qt::MidButton);
}

Item_Edge::~Item_Edge() {}
/*================================================================*/
/*                      Protected methods                         */
/*================================================================*/

void Item_Edge::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	emit signal_edge_clicked(event->scenePos(), &m_way, first(), second(), event->button());
	QGraphicsItem::mouseReleaseEvent(event);
}

void Item_Edge::handle_event_update(Osm_Node& node) {
	prepareGeometryChange();
	update(boundingRect());
}

/*================================================================*/
/*                        Public methods                          */
/*================================================================*/

int Item_Edge::type() const {
	return Type;
}

QRectF Item_Edge::boundingRect() const {
	double left = std::min(m_coord_handler.get_pos_on_scene(*first()).x(), m_coord_handler.get_pos_on_scene(*second()).x());
	double top = std::max(m_coord_handler.get_pos_on_scene(*first()).y(), m_coord_handler.get_pos_on_scene(*second()).y());
	double right = std::max(m_coord_handler.get_pos_on_scene(*first()).x(), m_coord_handler.get_pos_on_scene(*second()).x());
	double bottom = std::min(m_coord_handler.get_pos_on_scene(*first()).y(), m_coord_handler.get_pos_on_scene(*second()).y());

	return QRectF(QPointF(left, top), QPointF(right, bottom));
}

void Item_Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	QPen pen;

	pen.setWidth(1);
	painter->setPen(pen);
	painter->drawLine(m_coord_handler.get_pos_on_scene(*first()), m_coord_handler.get_pos_on_scene(*second()));
}


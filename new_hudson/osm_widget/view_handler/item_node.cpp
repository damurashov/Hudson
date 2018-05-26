#include "item_node.h"

using namespace ns_osm;

/*================================================================*/
/*                  Constructors, destructors                     */
/*================================================================*/

Item_Node::Item_Node(const Coord_Handler& handler,
                     Osm_Node& node,
                     QGraphicsItem* p_parent)
                     :
                       QGraphicsObject(p_parent),
                       m_coord_handler(handler),
                       m_node(node)
{
	setPos(m_coord_handler.get_pos_on_scene(node));
	setZValue(10);
	setFlag(ItemIsMovable);
	setFlag(ItemSendsGeometryChanges);
	setActive(true);
	setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton | Qt::MidButton);
}

Item_Node::~Item_Node() {}

/*================================================================*/
/*                      Protected methods                         */
/*================================================================*/

QVariant Item_Node::itemChange(GraphicsItemChange change, const QVariant &value) {
	QPointF pos = value.toPointF();
	pos = m_coord_handler.get_geo_coords(pos);
	prepareGeometryChange();
	switch (change) {
	case ItemPositionHasChanged:
		m_node.set_lat_lon(pos.y(), pos.x());
		break;
	case ItemPositionChange:
		m_node.set_lat_lon(pos.y(), pos.x());
		break;
	default:
		break;
	}
	return QGraphicsItem::itemChange(change, value);
}

void Item_Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	emit signal_node_clicked(&m_node, event->button());
	QGraphicsItem::mouseReleaseEvent(event);
}

/*================================================================*/
/*                        Public methods                          */
/*================================================================*/

Osm_Node* Item_Node::get_node() const {
	return &m_node;
}

int Item_Node::type() const {
	return Type;
}

void Item_Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	QPen pen;
	QBrush brush;

	brush.setColor(Qt::GlobalColor::darkBlue);
	brush.setStyle(Qt::BrushStyle::SolidPattern);
	pen.setColor(Qt::red);
	pen.setWidth(1);
	pen.setCosmetic(true);
	painter->setPen(pen);
	painter->setBrush(brush);
	painter->drawEllipse(QPoint(0,0), 2, 2);
}

QRectF Item_Node::boundingRect() const {
	return QRectF(-3, -3, 6, 6);
}


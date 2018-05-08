#include "item_node.h"

using namespace ns_osm;

/*================================================================*/
/*                  Constructors, destructors                     */
/*================================================================*/

Item_Node::Item_Node(const Osm_Map& map,
                     Osm_Node& node,
                     QGraphicsItem* p_parent)
                     :
                       QGraphicsObject(p_parent),
                       m_map(map),
                       m_node(node)
{
	setPos(m_map.get_scene_coord(&m_node));
	//setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);
}

Item_Node::~Item_Node() {}

/*================================================================*/
/*                      Protected methods                         */
/*================================================================*/

//int Item_Node::get_pen_size() const {
//	int size_px = (scale() < 2.0 ? 30 : 80);
//	return size_px;
//}

QVariant Item_Node::itemChange(GraphicsItemChange change, const QVariant &value) {
	QPointF pos = value.toPointF();
	pos = m_map.get_geo_coord(pos);
	switch (change) {
	case QGraphicsItem::ItemPositionHasChanged:
		m_node.set_lat_lon(pos.y(), pos.x());
		break;
	default:
		break;
	}
	return QGraphicsItem::itemChange(change, value);
}

void Item_Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	emit signal_node_clicked(&m_node, event->button());
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

	brush.setColor(Qt::GlobalColor::yellow);
	pen.setColor(Qt::red);
	pen.setWidth(20);
	pen.setCosmetic(true);
	painter->setPen(pen);
	painter->setBrush(brush);
	painter->drawEllipse(QPoint(0,0), 20, 20);
}

QRectF Item_Node::boundingRect() const {
	return QRectF(-20, 20, 40, 40);
}

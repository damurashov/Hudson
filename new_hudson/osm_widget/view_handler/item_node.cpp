#include "item_node.h"

using namespace ns_osm;

/*================================================================*/
/*                  Constructors, destructors                     */
/*================================================================*/

Item_Node::Item_Node(const Osm_Map& map,
                     Osm_Node& node,
                     QObject* p_parent)
                     : QObject(p_parent),
                       m_map(map),
                       m_node(node)
{
	setPos(m_map.get_scene_coord(mp_node));
	//setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

Item_Node::~Item_Node() {}

/*================================================================*/
/*                      Protected methods                         */
/*================================================================*/

int Item_Node::get_pen_size() const {
	int size_px = (scale() < 2.0 ? 4 : 10);
}

QVariant Item_Node::itemChange(GraphicsItemChange change, const QVariant &value) {
	QPointF pos = value.toPointF();
	switch (change) {
	case QGraphicsItem::ItemPositionHasChanged:
		m_node.set_lat_lon(pos.y(), pos.x());
	}
	return QGraphicsItem::itemChange(change, value);
}

void Item_Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	emit signal_node_clicked(&m_node, event->button());
}

/*================================================================*/
/*                        Public methods                          */
/*================================================================*/

int Item_Node::type() const {
	return Type;
}

void Item_Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	QPen pen;

	pen.setWidth(get_pen_size());
	pen.setCosmetic(true);
	painter->setPen(pen);
	painter->drawPoint(m_map.get_scene_coord(&m_node));
}

QRectF Item_Node::boundingRect() const {
	int sz = get_pen_size();
	return QRectF(-sz, sz, 2*sz, 2*sz);
}

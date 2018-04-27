#include "item_edge.h"

using namespace ns_osm;

/*================================================================*/
/*                  Constructors, destructors                     */
/*================================================================*/

Item_Edge::Item_Edge(const Osm_Map& map,
                     Osm_Way& way,
                     Osm_Node& node1,
                     Osm_Node& node2,
                     QObject *p_parent)
                     : QObject(p_parent),
                       m_map(map),
                       m_way(way),
                       m_node1(node1),
                       m_node2(node2)
{
	subscribe(&m_node1);
	subscribe(&m_node2);
}

Item_Edge::~Item_Edge() {}
/*================================================================*/
/*                      Protected methods                         */
/*================================================================*/

void Item_Edge::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	emit signal_edge_clicked(event->scenePos(), &m_way, &m_node1, &m_node2, event->button());
}

/*================================================================*/
/*                        Public methods                          */
/*================================================================*/

Osm_Node* Item_Edge::first() const {
	return &m_node1;
}

Osm_Node* Item_Edge::second() const {
	return &m_node2;
}

int Item_Edge::type() const {
	return Type;
}

void Item_Edge::handle_event_update(Osm_Node& node) {
	update(boundingRect());
}

QRectF Item_Edge::boundingRect() const {
	double left = std::min(m_map.get_scene_coord(&m_node1).x(), m_map.get_scene_coord(&m_node2).x());
	double top = std::max(m_map.get_scene_coord(&m_node1).y(), m_map.get_scene_coord(&m_node2).y());
	double right = std::max(m_map.get_scene_coord(&m_node1).x(), m_map.get_scene_coord(&m_node2).x());
	double bottom = std::min(m_map.get_scene_coord(&m_node1).y(), m_map.get_scene_coord(&m_node2).y());

	return QRectF(QPointF(left, top), QPointF(right, bottom));
}

void Item_Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	QPen pen;

	pen.setWidth(3);
	painter->setPen(pen);
	painter->drawLine(m_map.get_scene_coord(&m_node1), m_map.get_scene_coord(&m_node2));
}


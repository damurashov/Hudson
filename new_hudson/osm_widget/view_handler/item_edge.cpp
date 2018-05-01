#include "item_edge.h"

using namespace ns_osm;

/*================================================================*/
/*                  Constructors, destructors                     */
/*================================================================*/

Item_Edge::Item_Edge(const Osm_Map& map,
                     Osm_Node& node1,
                     Osm_Node& node2,
                     Osm_Way& way,
                     QObject* p_parent)
                     : QObject(p_parent),
                       m_map(map),
                       m_way(way),
                       mp_node_first(&node1),
                       mp_node_second(&node2)
{
	subscribe(*mp_node_first);
	subscribe(*mp_node_second);
}

Item_Edge::Item_Edge(const Osm_Map& map,
                     const Edge& edge,
                     Osm_Way& way,
                     QObject *p_parent)
                     : QObject(p_parent),
                       m_map(map),
                       m_way(way),
                       mp_node_first(edge.first()),
                       mp_node_second(edge.second())
{
	subscribe(*mp_node_first);
	subscribe(*mp_node_second);
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
	update(boundingRect());
}

/*================================================================*/
/*                        Public methods                          */
/*================================================================*/

int Item_Edge::type() const {
	return Type;
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


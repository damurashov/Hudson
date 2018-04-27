#ifndef ITEM_EDGE_H
#define ITEM_EDGE_H

#ifndef QT_WIDGETS_H
#define QT_WIDGETS_H
#include <QtWidgets>
#endif /* Include guard QT_WIDGETS_H */

#ifndef ALGORITHM_H
#define ALGORITHM_H
#include <algorithm>
#endif /* Include guard ALGORITHM_H */

#include "osm_elements.h"

namespace ns_osm {

class Item_Edge : public QGraphicsItem, public Osm_Subscriber {
	Q_OBJECT
signals:
	void			signal_edge_clicked	(QPointF,
	                                     Osm_Way*,
	                                     Osm_Node*,
	                                     Osm_Node*,
	                                     Qt::MouseButton);
protected:
	const Osm_Map&	m_map;
	Osm_Way&		m_way;
	Osm_Node&		m_node1;
	Osm_Node&		m_node2;

	void			mouseReleaseEvent	(QGraphicsSceneMouseEvent *event) override;
public:
	enum {Type = UserType + 2};

	Osm_Node*		first				() const;
	Osm_Node*		second				() const;
	int				type				() const override;
	void			handle_event_update	(Osm_Node&) override;
	QRectF			boundingRect		() const override;
	void			paint				(QPainter *painter,
	                                     const QStyleOptionGraphicsItem *option,
	                                     QWidget *widget) override;
	                Item_Edge			(const Osm_Map&,
					                     Osm_Way&,
					                     Osm_Node&,
					                     Osm_Node&,
					                     QObject* p_parent = nullptr);
					Item_Edge			() = delete;
					Item_Edge			(const Item_Edge&) = delete;
	Item_Edge&		operator=			(const Item_Edge&) = delete;
	virtual			~Item_Edge			();
};

}
#endif // ITEM_EDGE_H

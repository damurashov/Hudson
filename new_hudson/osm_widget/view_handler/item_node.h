#ifndef ITEM_NODE_H
#define ITEM_NODE_H

#ifndef QT_WIDGETS_H
#define QT_WIDGETS_H
#include <QtWidgets>
#endif /* Include guard QT_WIDGETS_H */

#include "osm_elements.h"

namespace ns_osm {

class Item_Node : public QGraphicsObject {
	Q_OBJECT
signals:
	void			signal_node_clicked	(Osm_Node*, Qt::MouseButton);
protected:
	const Osm_Map&	m_map;
	Osm_Node&		m_node;

//	int				get_pen_size		() const;
	QVariant		itemChange			(GraphicsItemChange change, const QVariant &value) override;
	void			mouseReleaseEvent	(QGraphicsSceneMouseEvent *event) override;
public:
	enum {Type = UserType + 1};

	Osm_Node*		get_node			() const;
	int				type				() const override;
	virtual void	paint				(QPainter *painter,
	                                     const QStyleOptionGraphicsItem *option,
	                                     QWidget *widget) override;
	QRectF			boundingRect		() const override;
	                Item_Node			(const Osm_Map&,
					                     Osm_Node&,
					                     QGraphicsItem* p_parent = nullptr);
					Item_Node			() = delete;
					Item_Node			(const Item_Node&) = delete;
	Item_Node&		operator=			(const Item_Node&) = delete;
	virtual			~Item_Node			();
};

}

#endif // ITEM_NODE_H

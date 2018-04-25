#ifndef ITEM_NODE_H
#define ITEM_NODE_H

#ifndef QT_WIDGETS_H
#define QT_WIDGETS_H
#include <QtWidgets>
#endif /* Include guard QT_WIDGETS_H */

#include "osm_elements.h"

namespace ns_osm {

class Item_Node : public QGraphicsEllipseItem{
	Q_OBJECT
signals:
	void		signal_node_clicked	(Osm_Node*);
protected:
	Osm_Node*	mp_node;

	QVariant	itemChange			(GraphicsItemChange change, const QVariant &value) override;
	void		mouseReleaseEvent	(QGraphicsSceneMouseEvent *event) override;
public:
	            Item_Node			(const Item_Node&);
				Item_Node			(const Item_Node&, QObject* p_parent = nullptr);
				Item_Node			(Osm_Node&, QObject* p_parent = nullptr);
				Item_Node			() = delete;
	Item_Node&	operator=			(const Item_Node&);
};

}

#endif // ITEM_NODE_H

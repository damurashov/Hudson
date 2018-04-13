#ifndef ITEM_NODE_H
#define ITEM_NODE_H

#include "osm_wgt.h"

namespace ns_osm {

class Item_Edge;

class Item_Node : public QGraphicsItem {
private:
	QList<Item_Edge*>	m_edges_list;
	Osm_Node*			p_node;
public:
	enum {Type = UserType + 1};

	const Osm_Node*		get_node	();
	int					type		() const override;
	QRectF				boundingRect() const override;
	void				paint		(QPainter* p_painter,
	                                 const QStyleOptionGraphicsItem* p_option,
	                                 QWidget* p_widget) override;
	                    Item_Node	(Osm_Node* p_node);
	virtual				~Item_Node	();
};/*class Item_Node*/

}/*namespace ns_osm*/

#endif // ITEM_NODE_H

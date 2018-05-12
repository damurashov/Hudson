#ifndef ITEM_WAY_H
#define ITEM_WAY_H

#ifndef QT_WIDGETS_H
#define QT_WIDGETS_H
#include <QtWidgets>
#endif /* Include guard QT_WIDGETS_H */

#include "osm_elements.h"
#include "item_edge.h"
#include "coord_handler.h"

namespace ns_osm {

class View_Handler;

class Item_Way : public QGraphicsItem, public Osm_Subscriber {
	//Q_OBJECT
private:
	struct Diff;
	mutable QList<Item_Edge*>	m_edges;
	QSet<Item_Edge*>			m_garbage;
//	const Osm_Map&				m_map;
	const Coord_Handler			m_coord_handler;
	View_Handler&				m_view_handler;
	Osm_Way&					m_way;

	bool						split_edge			(int item_edge_pos, Osm_Node* p_node_to_split_with);
	bool						merge_edges			(int item_edge_pos_prev, int item_edge_pos_next);
	int							seek_pos_node_first	(Osm_Node* p_first, Osm_Node* p_next = nullptr) const;
	int							seek_pos_item_edge	(Osm_Node* p_first, Osm_Node* p_second) const;
	Diff						get_diff			() const;
	void						handle_diffs		();
	void						handle_added_front	();
	void						handle_added_back	();
	void						handle_added_mid	(const Meta&);
	void						handle_deleted_front();
	void						handle_deleted_back	();
	void						handle_deleted_mid	(const Meta&);
	void						reg					(Item_Edge*);
	void						unreg				(Item_Edge*);
protected:
	void						handle_event_update	(Osm_Way&) override;
//	void						mouseReleaseEvent	(QGraphicsSceneMouseEvent *event) override;
public:
	enum						{Type = UserType + 3};

	QRectF						boundingRect		() const override;
	Osm_Way*					get_way				() const;
	void						paint				(QPainter *painter,
	                                                 const QStyleOptionGraphicsItem *option,
	                                                 QWidget *widget) override;
	int							type				() const override;
	                            Item_Way			(const Coord_Handler&,
								                     View_Handler& view_handler,
								                     Osm_Way& way,
								                     QGraphicsItem* p_parent = nullptr);
	virtual						~Item_Way			();
								Item_Way			()					= delete;
								Item_Way			(const Item_Way&)	= delete;
	Item_Way&					operator=			(const Item_Way&)	= delete;
};

/*================================================================*/
/*                        Item_Way::Diff                          */
/*================================================================*/

struct Item_Way::Diff {
	enum Diff_Type {ADD_BEFORE, REMOVE, NONE}	type;
	QList<Item_Edge*>::iterator					it_diff;
	Osm_Node*									p_node_first;
	Osm_Node*									p_node_second;
};

}

#endif // ITEM_WAY_H

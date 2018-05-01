#ifndef ITEM_WAY_H
#define ITEM_WAY_H

#ifndef QT_WIDGETS_H
#define QT_WIDGETS_H
#include <QtWidgets>
#endif /* Include guard QT_WIDGETS_H */

#include "osm_elements.h"
#include "item_edge.h"

namespace ns_osm {

class View_Handler;

/*================================================================*/
/*                           Item_Way                             */
/*================================================================*/

class Item_Way : public QGraphicsItemGroup, public Osm_Subscriber {
	Q_OBJECT
private:
	struct Osm_Edge;
	struct Diff;

	const Osm_Map&		m_map;
	Osm_Way&			m_way;
	QList<Item_Edge*>	m_edges;
protected:
	void				handle_event_update	(Osm_Way&) override;
public:
	enum {Type = UserType + 3};

	QList<Edge>			get_added			() const;
	QList<Item_Edge*>	get_removed			() const;
	Item_Edge*			get_item			(const Edge&) const;
	Item_Edge*			emplace_item		(const Edge&);
	void				remove_item			(Item_Edge*);
	void				paint				(QPainter *painter,
	                                         const QStyleOptionGraphicsItem *option,
	                                         QWidget *widget) override;
	int					type				() const override;
	                    Item_Way			(const Osm_Map& map,
						                     Osm_Way& way,
						                     QObject* p_parent = nullptr);
						Item_Way			() = delete;
						Item_Way			(const Item_Way&) = delete;
	Item_Way&			operator=			(const Item_Way&) = delete;
	virtual				~Item_Way			();
};

/*================================================================*/
/*                      Inner classifiers                         */
/*================================================================*/

#endif // ITEM_WAY_H
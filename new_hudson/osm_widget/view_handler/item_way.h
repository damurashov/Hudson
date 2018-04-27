#ifndef ITEM_WAY_H
#define ITEM_WAY_H

#ifndef QT_WIDGETS_H
#define QT_WIDGETS_H
#include <QtWidgets>
#endif /* Include guard QT_WIDGETS_H */

#include "osm_elements.h"
#include "item_edge.h"

namespace ns_osm {

class Item_Way : public QGraphicsItemGroup, public Osm_Subscriber {
	Q_OBJECT
protected:
	const Osm_Map&		m_map;
	Osm_Way&			m_way;
	QList<Item_Edge*>	m_edges;

	void				compose_group		();
public:
	enum {Type = UserType + 3};

	void				paint				(QPainter *painter,
	                                         const QStyleOptionGraphicsItem *option,
	                                         QWidget *widget) override;
	int					type				() const override;
	void				handle_event_update	(Osm_Way&) override;
	                    Item_Way			(const Osm_Map&, Osm_Way&, QObject* p_parent = nullptr);
						Item_Way			() = delete;
						Item_Way			(const Item_Way&) = delete;
	Item_Way&			operator=			(const Item_Way&) = delete;
	virtual				~Item_Way			();
};

}

#endif // ITEM_WAY_H

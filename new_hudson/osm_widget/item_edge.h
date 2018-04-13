#ifndef ITEM_EDGE_H
#define ITEM_EDGE_H

#include "osm_wgt.h"

namespace ns_osm {

class Item_Edge : public QGraphicsItem {
private:
	Item_Node*	mp_item_node_begin;
	Item_Node*	mp_item_node_end;
	Osm_Way*	mp_osm_way;
public:
	enum {Type = UserType + 2};
	int type() const override;
	Item_Edge();
}; /* class Item_Edge */

} /* namespace ns_osm */

#endif // ITEM_EDGE_H

#include "item_way.h"
#include "view_handler.h"

using namespace ns_osm;

/*================================================================*/
/*                  Constructors, destructors                     */
/*================================================================*/
Item_Way::Item_Way(const Osm_Map& map,
                   Osm_Way& way,
                   QObject* p_parent)
                   : m_map(map),
                     m_way(way)
{
	subscribe(m_way);
}
/*================================================================*/
/*                      Protected methods                         */
/*================================================================*/

void Item_Way::handle_event_update(Osm_Way&) {
	update();
}

/*================================================================*/
/*                        Public methods                          */
/*================================================================*/

QList<Edge> Item_Way::get_added() const {
	QList<Edge> actual(Edge::to_edge_list(m_way));
	QList<Edge>	to_be_emplaced;

	for (auto it_old = m_edges.begin(); it_old != m_edges.end(); ++it_old) {
		for (auto it_actual = actual.begin(); it_actual != actual.end(); ++it_actual) {
			if (*it_actual == **it_old) {
				break;
			}
		}
		if (it_actual == actual.end()) {
			to_be_emplaced.push_back(*it_actual);
		}
	}

	return to_be_emplaced;
}

QList<Item_Edge*> Item_Way::get_removed() const {
	QList<Item_Edge*>	to_be_removed;
	QList<Edge>			actual(Edge::to_edge_list(m_way));

	for (auto it_actual = actual.begin(); it_actual != actual.end(); ++it_actual) {
		for (auto it_old = m_edges.begin(); it_old != m_edges.end(); ++it_old) {
			if (**it_old == *it_actual) {
				break;
			}
		}
		if (it_old == m_edges.end()) {
			to_be_removed.push_back(*it_old);
		}
	}

	return to_be_removed;
}

Item_Edge* Item_Way::get_item(const Edge& edge) const {
	for (auto it = m_edges.begin(); it != m_edges.end(); ++it) {
		if (**it == edge) {
			return *it;
		}
	}
	return nullptr;
}

Item_Edge* Item_Way::emplace_item(const Edge& edge) {
	if (get_item(edge) != nullptr) {
		return nullptr;
	}

	m_edges.push_back(new Item_Edge(m_map, edge, m_way));
	addToGroup(m_edges.back());

	return m_edges.back();
}

void Item_Way::remove_item(Item_Edge* p_item_edge) {
	if (get_item(*p_item_edge) == nullptr) {
		return;
	}

	removeFromGroup(p_item_edge);
	m_edges.removeOne(p_item_edge);
	delete p_item_edge;
}

int Item_Way::type() const {
	return Type;
}

#include <QtTest>
#include "osm_widget.h"

using namespace ns_osm;

class Test_Item_Way : public QObject {
	Q_OBJECT
private slots:
	void newly_added_empty_way() {
		Osm_Widget widget;
		Osm_Map& map = *(widget.mp_map);
		Osm_Way* p_way = new Osm_Way;

		map.add(p_way);
		QCOMPARE(false, widget.mp_view_handler->m_wayid_to_item.empty());
		QCOMPARE(true, widget.mp_view_handler->m_wayid_to_item.begin().value()->m_edges.empty());

		map.remove(p_way);
		QCOMPARE(true, widget.mp_view_handler->m_wayid_to_item.empty());
	}

	void newly_added_not_empty_way___size() {
		const int N_NODES = 10;
		Osm_Widget widget;
		Osm_Map& map = *(widget.mp_map);
		Osm_Node* ap_nodes[N_NODES];
		Osm_Way* p_way = new Osm_Way;
//		Osm_Node* p_intermediate_node = new Osm_Node(30.30, 30.30);

		for (int i = 0; i < N_NODES; ++i) {
			ap_nodes[i] = new Osm_Node(i,i);
			p_way->push_node(ap_nodes[i]);
		}

		map.add(p_way);

		/* Size */
		QCOMPARE(N_NODES-1, widget.mp_view_handler->m_wayid_to_item.begin().value()->m_edges.size());
		p_way->push_node(ap_nodes[0]);
		QCOMPARE(N_NODES, widget.mp_view_handler->m_wayid_to_item.begin().value()->m_edges.size());
	}

	void newly_added_not_empty_way___order___node_added_between_back_back_to_close() {
		const int N_NODES = 10;
		Osm_Widget widget;
		Osm_Map& map = *(widget.mp_map);
		Osm_Node* ap_nodes[N_NODES];
		Osm_Way* p_way = new Osm_Way;
		Osm_Node* p_intermediate_node = new Osm_Node(30.30, 30.30);
		int counter = 0;

		for (int i = 0; i < N_NODES; ++i) {
			ap_nodes[i] = new Osm_Node(i,i);
			p_way->push_node(ap_nodes[i]);
		}

		map.add(p_way);

		QList<Item_Edge*>& edgelist = widget.mp_view_handler->m_wayid_to_item.begin().value()->m_edges;
		QCOMPARE(ap_nodes[0], edgelist.front()->first());
		for (auto it = edgelist.begin(); it != edgelist.end(); ++it) {
			counter++;
			QCOMPARE(ap_nodes[counter], (*it)->second());
		}

		p_way->insert_node_between(p_intermediate_node, ap_nodes[1], ap_nodes[2]);
		QCOMPARE(p_way->get_nodes_list().front(), edgelist.front()->first());
		auto it_waynode = p_way->get_nodes_list().cbegin();
		for (auto it = edgelist.begin(); it != edgelist.end(); ++it) {
			it_waynode++;
			QCOMPARE(*it_waynode, (*it)->second());
		}
	}

	void node_delete_from_non_closed_way___order() {
		const int N_NODES = 4;
		Osm_Widget widget;
		Osm_Map& map = *(widget.mp_map);
		Osm_Way* p_way = new Osm_Way;
		Osm_Node* ap_nodes[N_NODES];
		map.add(p_way);
		QList<Item_Edge*>& itemedges = widget.mp_view_handler->m_wayid_to_item.begin().value()->m_edges;
		const QList<Osm_Node*>& waynodes = p_way->get_nodes_list();

		for (int i = 0; i < N_NODES; ++i) {
			ap_nodes[i] = new Osm_Node(i,i);
			p_way->push_node(ap_nodes[i]);
		}

		delete ap_nodes[N_NODES-1];
		delete ap_nodes[1];
		auto it_node = waynodes.cbegin();
		auto it_edge = itemedges.begin();
		QCOMPARE(*it_node, (*it_edge)->first());
		it_node++;
		while (it_node != waynodes.cend() || it_edge != itemedges.end()) {
			QCOMPARE(*it_node, (*it_edge)->second());
			it_node++;
			it_edge++;
		}
	}

	void node_delete_from_closed_triangle_way___order() {
		const int N_NODES = 3;
		Osm_Widget widget;
		Osm_Map& map = *(widget.mp_map);
		Osm_Node* ap1_nodes[N_NODES];
		Osm_Node* ap2_nodes[N_NODES];
		Osm_Way* p1_way = new Osm_Way;
		Osm_Way* p2_way = new Osm_Way;
		map.add(p1_way);
		map.add(p2_way);
		QList<Item_Edge*>& edges1 = widget.mp_view_handler->m_wayid_to_item[p1_way->get_id()]->m_edges;
		QList<Item_Edge*>& edges2 = widget.mp_view_handler->m_wayid_to_item[p2_way->get_id()]->m_edges;
		const QList<Osm_Node*>& nodes1 = p1_way->get_nodes_list();
		const QList<Osm_Node*>& nodes2 = p2_way->get_nodes_list();

		for (int i = 0; i < N_NODES; ++i) {
			p1_way->push_node(ap1_nodes[i] = new Osm_Node(i,i));
			p2_way->push_node(ap2_nodes[i] = new Osm_Node(i,i));
		}
		p1_way->push_node(ap1_nodes[0]);
		p2_way->push_node(ap2_nodes[0]);

		delete ap1_nodes[0]; /* 0-1-2-0 -> 1-2 */
		delete ap2_nodes[1]; /* 0-1-2-0 -> 0-2 */

		QCOMPARE(nodes1.front(), edges1.front()->first());
		QCOMPARE(nodes1.back(), edges1.front()->second());
		QCOMPARE(1, edges1.size());
		QCOMPARE(nodes1.front(), ap1_nodes[1]);
		QCOMPARE(nodes1.back(), ap1_nodes[2]);

		QCOMPARE(nodes2.front(), edges2.front()->first());
		QCOMPARE(nodes2.back(), edges2.front()->second());
		QCOMPARE(1, edges2.size());
		QCOMPARE(nodes2.front(), ap2_nodes[0]);
		QCOMPARE(nodes2.back(), ap2_nodes[2]);
	}
};

QTEST_MAIN(Test_Item_Way)
#include "test_item_way.moc"

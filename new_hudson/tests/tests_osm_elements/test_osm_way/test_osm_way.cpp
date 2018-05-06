#include <QtTest>
#include "osm_elements.h"
using namespace ns_osm;
class Test_Osm_Way : public QObject {
	Q_OBJECT
private slots:
	void get_size() {
		const int ARRAY_SIZE = 4;
		Osm_Way* p_way = new Osm_Way;
		Osm_Node* ap_nodes[ARRAY_SIZE];

		for (int i = 0; i < ARRAY_SIZE; ++i) {
			ap_nodes[i] = new Osm_Node(i,i);
			p_way->push_node(ap_nodes[i]);
		}

		QCOMPARE(ARRAY_SIZE, p_way->get_size());
		delete ap_nodes[ARRAY_SIZE-1];
		QCOMPARE(ARRAY_SIZE-1, p_way->get_size());

		delete p_way;
		for (int i = 0; i < ARRAY_SIZE-1; ++i) {
			delete ap_nodes[i];
		}
	}

	void get_capacity() {
		Osm_Way* p_way = new Osm_Way;

		QCOMPARE(2000, p_way->get_capacity());
		delete p_way;
	}

	void push_node() {
		const int ARRAY_SIZE = 4;
		Osm_Way* p_way = new Osm_Way;
		Osm_Node* ap_nodes[ARRAY_SIZE];

		for (int i = 0; i < ARRAY_SIZE; ++i) {
			ap_nodes[i] = new Osm_Node(i,i);
			QCOMPARE(true, p_way->push_node(ap_nodes[i]));
		}
		QCOMPARE(false, p_way->push_node(ap_nodes[ARRAY_SIZE-1]));
		QCOMPARE(true, p_way->push_node(ap_nodes[0]));
		QCOMPARE(false, p_way->push_node(ap_nodes[1]));

		for (int i = 0; i < ARRAY_SIZE; ++i) {
			delete ap_nodes[i];
		}
		delete p_way;
	}

	void insert_node_between() {
		const int ARRAY_SIZE = 5;
		Osm_Way* p_way = new Osm_Way;
		Osm_Node* ap_nodes[ARRAY_SIZE];
		for (int i = 0; i < ARRAY_SIZE; ++i) {
			ap_nodes[i] = new Osm_Node(i,i);
			if (i != ARRAY_SIZE - 1) {
				p_way->push_node(ap_nodes[i]);
			}
		}

		QCOMPARE(false, p_way->insert_node_between(ap_nodes[0], ap_nodes[1], ap_nodes[2])); // Already has inside
		QCOMPARE(false, p_way->insert_node_between(ap_nodes[ARRAY_SIZE - 1], ap_nodes[0], ap_nodes[2])); // Not adjacent
		QCOMPARE(false, p_way->insert_node_between(ap_nodes[ARRAY_SIZE - 1], ap_nodes[1], nullptr)); // nullptr
		QCOMPARE(false, p_way->insert_node_between(ap_nodes[ARRAY_SIZE - 1], nullptr, ap_nodes[1])); // nullptr
		QCOMPARE(true, p_way->insert_node_between(ap_nodes[ARRAY_SIZE - 1], ap_nodes[2], ap_nodes[3]));

		delete p_way;
		for (int i = 0; i < ARRAY_SIZE; ++i) {
			delete ap_nodes[i];
		}
	}

	void has() {
		Osm_Way* p_way = new Osm_Way;
		Osm_Node* p1_node = new Osm_Node(1.0, 1.0);
		Osm_Node* p2_node = new Osm_Node(2.0, 2.0);
		Osm_Node* p3_node = nullptr; /* nullptr */

		QCOMPARE(false,p_way->has(p1_node));
		QCOMPARE(false,p_way->has(p3_node));

		p_way->push_node(p1_node);
		p_way->push_node(p2_node);
		QCOMPARE(2, p_way->get_size());
		QCOMPARE(true, p_way->is_valid());

		QCOMPARE(false, p_way->push_node(p3_node));
		QCOMPARE(false, p_way->is_valid());
		QCOMPARE(2, p_way->get_size());

		delete p1_node;
		QCOMPARE(1, p_way->get_size());
		delete p2_node;
		QCOMPARE(0, p_way->get_size());
		QCOMPARE(true, p_way->is_empty());
		delete p_way;
	}

	void is_closed() {
		const int N_NODES = 5;
		const int N_REMAINS = 3;
		Osm_Way way;
		Osm_Node* ap_nodes[N_NODES];

		for (int i = 0; i < N_NODES; ++i) {
			ap_nodes[i] = new Osm_Node(i,i);
			way.push_node(ap_nodes[i]);
		}
		QCOMPARE(false, way.is_closed());
		way.push_node(ap_nodes[0]);
		QCOMPARE(true, way.is_closed());

		for (int i = 0; i < N_NODES - N_REMAINS; ++i) {
			delete ap_nodes[i];
			QCOMPARE(true, way.is_closed());
		}
		delete ap_nodes[N_NODES - N_REMAINS];
		QCOMPARE(false, way.is_closed());
	}
};

QTEST_MAIN(Test_Osm_Way)
#include "test_osm_way.moc"

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
		QCOMPARE(false, p_way->push_node(ap_nodes[0]));

		for (int i = 0; i < ARRAY_SIZE; ++i) {
			delete ap_nodes[i];
		}
		delete p_way;
	}

	void insert_node_between() {
		const int ARRAY_SIZE = 3;
		Osm_Way* p_way = new Osm_Way;
		Osm_Node* ap_nodes[ARRAY_SIZE];
		for (int i = 0; i < ARRAY_SIZE; ++i) {
			ap_nodes[i] = new Osm_Node(i,i);
		}

		QCOMPARE(false, p_way->insert_node_between(ap_nodes[0], ap_nodes[1], ap_nodes[2]));;
		p_way->push_node(ap_nodes[0]);
		QCOMPARE(true, p_way->insert_node_between(ap_nodes[2], ap_nodes[0], ap_nodes[1]));
		QCOMPARE(true, p_way->insert_node_between(ap_nodes[1], ap_nodes[0], ap_nodes[2]));
		QCOMPARE(3, p_way->get_size());

		delete p_way;
		for (int i = 0; i < ARRAY_SIZE; ++i) {
			delete ap_nodes[i];
		}
	}
};

QTEST_MAIN(Test_Osm_Way)
#include "test_osm_way.moc"

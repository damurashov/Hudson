#include <QtTest>
#include "osm_elements.h"
using namespace ns_osm;
class Test_Osm_Way : public QObject {
	Q_OBJECT
private slots:

	void get_size() {
		Osm_Way way;
		Osm_Node* ptr_node_1 = new Osm_Node(1.1,1.1);
		//Osm_Node* ptr_node_2 = new Osm_Node(2.2, 2.2);

		QCOMPARE(0, way.get_size());

		way.push_node(ptr_node_1);
		//way.push_node(ptr_node_2);
		QCOMPARE(1, way.get_size());

		delete ptr_node_1;
		QCOMPARE(0, way.get_size());
	}

	void get_capacity() {
		Osm_Way way;
		QCOMPARE(2000, way.get_capacity());
	}

	void push_node() {
		//bool f_success;
		Osm_Way way;
		Osm_Node* ptr_node_1 = new Osm_Node(1.1, 1.1);
		Osm_Node* ptr_node_2 = new Osm_Node(2.2, 2.2);
		Osm_Node* ptr_node_3 = new Osm_Node(3.3, 3.3);

		QCOMPARE(0, way.get_size());

		way.push_node(ptr_node_1);
		way.push_node(ptr_node_2);
		way.push_node(ptr_node_3);
		QCOMPARE(3, way.get_size());

		QCOMPARE(false, way.push_node(ptr_node_3));
		QCOMPARE(3, way.get_size());

		QCOMPARE(true, way.push_node(ptr_node_1));
		QCOMPARE(4, way.get_size());

		delete ptr_node_1;
		QCOMPARE(2, way.get_size());

		delete ptr_node_2;
		delete ptr_node_3;
		QCOMPARE(0, way.get_size());
	}

	void insert_node_between() {
		Osm_Way way;
		Osm_Node* ptr_node_1 = new Osm_Node(1.1, 1.1);
		Osm_Node* ptr_node_2 = new Osm_Node(2.2, 2.2);
		Osm_Node* ptr_node_3 = new Osm_Node(3.3, 3.3);

		QCOMPARE(0, way.get_size());

		QCOMPARE(false, way.insert_node_between(nullptr, nullptr, nullptr));

		QCOMPARE(false, way.insert_node_between(ptr_node_1, ptr_node_2, ptr_node_3));
		QCOMPARE(0, way.get_size());

		way.push_node(ptr_node_1);
		QCOMPARE(false, way.insert_node_between(ptr_node_1, ptr_node_1, ptr_node_2));
		QCOMPARE(1, way.get_size());

		QCOMPARE(true, way.insert_node_between(ptr_node_2, ptr_node_1, ptr_node_3));

		QCOMPARE(true, way.insert_node_between(ptr_node_3, ptr_node_1, ptr_node_2));

		QCOMPARE(ptr_node_3, way.get_nodes_list().at(1));
		QCOMPARE(3, way.get_size());
	}

	void has_node() {
		Osm_Way way;
		Osm_Node* ptr_node_1 = new Osm_Node(1.1, 1.1);
		Osm_Node* ptr_node_2 = new Osm_Node(2.2, 2.2);
		Osm_Node* ptr_node_3;
		int id_1 = ptr_node_1->get_id();
		int id_2 = ptr_node_2->get_id();

		QCOMPARE(false, way.has_node(nullptr));

		way.push_node(ptr_node_1);
		way.push_node(ptr_node_2);
		//way.push_node(ptr_node_3);
		QCOMPARE(true, way.has_node(ptr_node_1));
		//QCOMPARE(false, way.has_node(ptr_node_3));
	}

	void is_closed() {
		Osm_Way way;
		Osm_Node* ptr_node_1 = new Osm_Node(1.1, 1.1);
		Osm_Node* ptr_node_2 = new Osm_Node(2.2, 2.2);
		Osm_Node* ptr_node_3 = new Osm_Node(3.3, 3.3);

		QCOMPARE(false, way.is_closed());

		way.push_node(ptr_node_1);
		way.push_node(ptr_node_2);
		way.push_node(ptr_node_3);
		QCOMPARE(false, way.push_node(ptr_node_3));
		QCOMPARE(false, way.is_closed());

		way.push_node(ptr_node_1);
		QCOMPARE(true, way.is_closed());

		delete ptr_node_2;
		delete ptr_node_3;
		QCOMPARE(false, way.is_closed());
	}

	void is_valid() {
		Osm_Way way;
		Osm_Node* ptr_node_1 = new Osm_Node(1.1, 1.1);
		Osm_Node* ptr_node_2;
		Osm_Node* ptr_node_3 = new Osm_Node(QString::number(2.2), QString::number(2.2));
		int nonexst_id = (ptr_node_1->get_id()) + (ptr_node_2->get_id()) + 1;

		QCOMPARE(true, way.is_valid());

		way.push_node(ptr_node_1);
		QCOMPARE(true, way.is_valid());

		way.push_node(nullptr);
		QCOMPARE(false, way.is_valid());

		//QCOMPARE(true, way.push_node(ptr_node_2));
		//QCOMPARE(false, way.is_valid());

		way.push_node(ptr_node_3);
		QCOMPARE(false, way.is_valid());
	}

	void is_empty() {
		Osm_Way way;
		Osm_Node* ptr_node_1 = new Osm_Node(1.1, 1.1);
		Osm_Node* ptr_node_2 = new Osm_Node(2.2, 2.2);

		QCOMPARE(true, way.is_empty());

		way.push_node(ptr_node_1);
		way.push_node(ptr_node_2);
		QCOMPARE(false, way.is_empty());

		delete ptr_node_1;
		delete ptr_node_2;
		QCOMPARE(true, way.is_empty());
	}

	void get_nodes_list() {
		Osm_Way way;
		Osm_Node* ptr_node_1 = new Osm_Node(1.1, 1.1);
		Osm_Node* ptr_node_2 = new Osm_Node(2.2, 2.2);
		Osm_Node* ptr_node_3 = new Osm_Node(3.3, 3.3);
		Osm_Node* ptr_node_4 = new Osm_Node(4.4, 4.4);

		way.push_node(ptr_node_1);
		way.push_node(ptr_node_2);
		way.push_node(ptr_node_3);
		way.push_node(ptr_node_4);
		way.push_node(ptr_node_1);

		delete ptr_node_1;
		QCOMPARE(ptr_node_2, way.get_nodes_list().at(0));
		QCOMPARE(ptr_node_3, way.get_nodes_list().at(1));
		QCOMPARE(ptr_node_4, way.get_nodes_list().at(2));
	}
};

QTEST_MAIN(Test_Osm_Way)
#include "test_osm_way.moc"

#include "osm_elements.h"
#include <QtTest>
using namespace ns_osm;
class Test_Osm_Node : public QObject {
	Q_OBJECT
private slots:

	void is_valid() {
		Osm_Node* ptr_node = new Osm_Node(22.3, 44.1);
		QCOMPARE(true, ptr_node->is_valid());
	}

	void get_lat() {
		Osm_Node* ptr_node = new Osm_Node(QString("44.1"), QString("36.6"));
		QCOMPARE(44.1, ptr_node->get_lat());
	}

	void get_lon() {
		Osm_Node* ptr_node = new Osm_Node(QString("44.1"), QString("36.6"));
		QCOMPARE(36.6, ptr_node->get_lon());
	}

	void set_lat() {
		Osm_Node node(42, 42);
		QCOMPARE(42, node.get_lat());
		node.set_lat(42.42);
		QCOMPARE(42.42, node.get_lat());
	}

	void set_lon() {
		Osm_Node node(12, 12);
		QCOMPARE(12, node.get_lon());
		node.set_lon(42.42);
		QCOMPARE(42.42, node.get_lon());
	}

};

QTEST_MAIN(Test_Osm_Node)
#include "test_osm_node.moc"

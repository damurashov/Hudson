#include "osm_elements.h"
#include <QtTest>
using namespace ns_osm;
class Test_Osm_Relation : public QObject {
	Q_OBJECT
private slots:

	void add() {
		Osm_Relation relation;
		Osm_Way* p_way = new Osm_Way;
		Osm_Node* p_node = new Osm_Node(QString::number(12.2), QString::number(43.12));

		relation.add(p_way);
		relation.add(p_node);
		QCOMPARE(2, relation.get_size());
		QCOMPARE(1, relation.count_nodes());
		QCOMPARE(1, relation.count_ways());

		relation.add(p_node);
		relation.add(nullptr);
		QCOMPARE(2, relation.get_size());
		QCOMPARE(1, relation.count_nodes());
	}

	void remove() {
		Osm_Relation relation;
		Osm_Way* p_way = new Osm_Way;
		Osm_Node* p_node = new Osm_Node(1.1, 1.1);

		relation.add(p_way);
		relation.add(p_node);
		QCOMPARE(2, relation.get_size());

		relation.remove(p_way);
		relation.remove(nullptr);
		QCOMPARE(1, relation.get_size());
		delete p_node;
		QCOMPARE(0, relation.get_size());
	}

	void has_object() {
		Osm_Relation relation;
		Osm_Way* p_way = new Osm_Way;
		Osm_Node* p_node = new Osm_Node(1.1, 1.1);
		Osm_Node* p_node_2 = new Osm_Node(2.2, 2.2);

		relation.add(p_way);
		relation.add(p_node);
		relation.add(p_node_2);
		QCOMPARE(true, relation.has_object(p_way));
		QCOMPARE(true, relation.has_object(p_node));
		QCOMPARE(true, relation.has_object(p_node_2));

		relation.remove(p_way);
		delete p_node_2;
		QCOMPARE(false, relation.has_object(p_way));
		QCOMPARE(false, relation.has_object(p_node_2));
		QCOMPARE(true, relation.has_object(p_node));

		delete p_node;
		QCOMPARE(false, relation.has_object(p_node));
	}

	void set_role() {
		Osm_Relation relation;
		Osm_Way* p_way = new Osm_Way;
		Osm_Node* p_node = new Osm_Node(1.1, 1.1);

		relation.add(p_way, QString("busway"));
		relation.add(p_node);
		relation.set_role(p_node, QString("bus_stop"));
	}

	void get_size() {
		Osm_Relation relation;
		Osm_Way* ptr_way = new Osm_Way;
		Osm_Node* ptr_node = new Osm_Node(1.1, 1.1);

		QCOMPARE(0, relation.get_size());

		relation.add(ptr_way);
		relation.add(ptr_node, QString("some_role"));
		QCOMPARE(2, relation.get_size());

		relation.remove(ptr_way);
		QCOMPARE(1, relation.get_size());

		delete ptr_node;
		QCOMPARE(0, relation.get_size());
	}

	void get_role() {
		Osm_Relation rel;
		Osm_Node* p_node = new Osm_Node(1.1, 1.1);
		Osm_Way* p_way = new Osm_Way;

		QCOMPARE(QString(""), rel.get_role(p_node));

		rel.add(p_node, QString("this_is_my_role"));
		rel.add(p_way);
		QCOMPARE(QString("this_is_my_role"), rel.get_role(p_node));
		QCOMPARE(QString(""), rel.get_role(p_way));

		rel.set_role(p_way, QString("some_role"));
		QCOMPARE(QString("some_role"), rel.get_role(p_way));

		rel.remove(p_way);
		QCOMPARE(QString(""), rel.get_role(p_way));

		delete p_node;
		QCOMPARE(QString(""), rel.get_role(p_node));
	}

	void get_nodes() {
		Osm_Relation relation;
		Osm_Node* p_node_1 = new Osm_Node(11.1, 1.11);
		Osm_Node* p_node_2 = new Osm_Node(2.22, 222.2);

		QCOMPARE(true, relation.get_nodes().isEmpty());

		relation.add(p_node_1);
		relation.add(p_node_2);
		QCOMPARE(false, relation.get_nodes().isEmpty());
		QCOMPARE(p_node_1, relation.get_nodes().at(0));
		QCOMPARE(p_node_2, relation.get_nodes().at(1));
	}

	void get_ways() {
		Osm_Relation relation;
		Osm_Way* p_way_1 = new Osm_Way;
		Osm_Way* p_way_2 = new Osm_Way;

		QCOMPARE(true, relation.get_ways().isEmpty());

		relation.add(p_way_1);
		relation.add(p_way_2);
		QCOMPARE(false, relation.get_ways().isEmpty());
		QCOMPARE(p_way_1, relation.get_ways().at(0));
		QCOMPARE(p_way_2, relation.get_ways().at(1));
	}

	void get_relations() {
		Osm_Relation relation;
		Osm_Relation* p_rel_1 = new Osm_Relation;
		Osm_Relation* p_rel_2 = new Osm_Relation;

		QCOMPARE(true, relation.get_relations().isEmpty());

		relation.add(p_rel_1);
		relation.add(p_rel_2);
		QCOMPARE(false, relation.get_relations().isEmpty());
		QCOMPARE(p_rel_1, relation.get_relations().at(0));
		QCOMPARE(p_rel_2, relation.get_relations().at(1));
	}

	void count_nodes() {
		Osm_Relation relation;
		Osm_Node* p_node_1 = new Osm_Node(1.1, 11.1);
		Osm_Node* p_node_2 = new Osm_Node(22.2, 2.22);
		Osm_Way* p_way = new Osm_Way;

		QCOMPARE(0, relation.count_nodes());

		relation.add(p_node_1);
		relation.add(p_way);
		relation.add(p_node_2);
		QCOMPARE(3, relation.get_size());
		QCOMPARE(2, relation.count_nodes());

		relation.remove(p_node_1);
		delete p_node_2;
		QCOMPARE(1, relation.get_size());
		QCOMPARE(0, relation.count_nodes());
	}

	void count_ways() {
		Osm_Relation rel;
		Osm_Way* p_way_1 = new Osm_Way;
		Osm_Way* p_way_2 = new Osm_Way;
		Osm_Node* p_node = new Osm_Node(1.11, 1.11);
		Osm_Relation* p_rel = new Osm_Relation;

		QCOMPARE(0, rel.get_size());
		QCOMPARE(0, rel.count_ways());

		rel.add(p_way_1);
		rel.add(p_way_2);
		rel.add(p_node);
		rel.add(p_rel);
		QCOMPARE(4, rel.get_size());
		QCOMPARE(2, rel.count_ways());

		rel.remove(p_way_1);
		delete p_way_2;
		QCOMPARE(2, rel.get_size());
		QCOMPARE(0, rel.count_ways());
	}

	void count_relations() {
		Osm_Relation rel;
		Osm_Relation* p_rel_1 = new Osm_Relation;
		Osm_Relation* p_rel_2 = new Osm_Relation;
		Osm_Node* p_node = new Osm_Node(111.1, 1.11);
		Osm_Way* p_way = new Osm_Way;

		QCOMPARE(0, rel.get_size());
		QCOMPARE(0, rel.count_relations());

		rel.add(p_rel_1);
		rel.add(p_node);
		rel.add(p_rel_2);
		rel.add(p_way);
		QCOMPARE(4, rel.get_size());
		QCOMPARE(2, rel.count_relations());

		delete p_rel_1;
		rel.remove(p_rel_2);
		QCOMPARE(2, rel.get_size());
		QCOMPARE(0, rel.count_relations());
	}

	void is_valid() {
		Osm_Relation rel;
		Osm_Node* p_node = new Osm_Node(1.1, 1.1);
		long long id = p_node->get_id();

		QCOMPARE(true, rel.is_valid());

		rel.add(p_node);
		QCOMPARE(true, rel.is_valid());

		rel.add(nullptr);
		QCOMPARE(false, rel.is_valid());
	}
};

QTEST_MAIN(Test_Osm_Relation)
#include "test_osm_relation.moc"

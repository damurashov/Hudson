#include "osm_elements.h"
#include <QtTest>
using namespace ns_osm;
class Test_Osm_Relation : public QObject {
	Q_OBJECT
private slots:

	void add() {
		Osm_Relation* p_relation = new Osm_Relation;
		Osm_Way* p_way = new Osm_Way;
		Osm_Way* p_way2 = nullptr;
		Osm_Node* p_node = new Osm_Node(QString::number(12.2), QString::number(43.12));

		p_relation->add(p_way);
		p_relation->add(p_node);
		QCOMPARE(2, p_relation->get_size());
		QCOMPARE(1, p_relation->count_nodes());
		QCOMPARE(1, p_relation->count_ways());

		p_relation->add(p_node);
		p_relation->add(p_way2);
		QCOMPARE(2, p_relation->get_size());
		QCOMPARE(1, p_relation->count_nodes());

		delete p_way;
		delete p_relation;
		delete p_node;
	}

	void remove() {
		Osm_Relation* p_rel = new Osm_Relation;
		Osm_Way* p_way = new Osm_Way;
		Osm_Node* p_node = new Osm_Node(0.0,0.0);

		p_rel->add(p_way);
		p_rel->add(p_node);
		QCOMPARE(2, p_rel->get_size());
		delete p_way;
		p_rel->remove(p_node);
		QCOMPARE(0, p_rel->get_size());
		delete p_node;
		delete p_rel;
	}

	void has() {
		Osm_Relation rel;
		Osm_Way way1;
		Osm_Way way2;
		Osm_Node node(0,0);

		rel.add(&way1);
		rel.add(&way2);
		rel.add(&node);
		QCOMPARE(3, rel.get_size());
		QCOMPARE(true, rel.has(&way1));
		QCOMPARE(true, rel.has(&way2));
		QCOMPARE(true, rel.has(&node));
	}

	void get_role() {
		Osm_Relation* p_rel = new Osm_Relation;
		Osm_Way* p1_way = new Osm_Way;
		Osm_Way* p2_way = new Osm_Way;

		QCOMPARE("", p_rel->get_role(p1_way));
		p_rel->add(p1_way, "p1_way");
		p_rel->add(p2_way, "p2_way");
		QCOMPARE("p1_way", p_rel->get_role(p1_way));
		p_rel->set_role(p2_way, "another way");
		QCOMPARE("another way", p_rel->get_role(p2_way));
	}

	void count_nodes___count_ways___count_relations___get_size() {
		const int N_OBJECTS = 1000;
		const int N_TO_DELETE = 100;
		Osm_Relation relation;
		Osm_Node* ap_nodes[N_OBJECTS];
		Osm_Way* ap_ways[N_OBJECTS];
		Osm_Relation* ap_relations[N_OBJECTS];

		for (int i = 0; i < N_OBJECTS; ++i) {
			ap_nodes[i] = new Osm_Node(i,i);
			ap_ways[i] = new Osm_Way;
			ap_relations[i] = new Osm_Relation;
			relation.add(ap_nodes[i], QString::number(i));
			relation.add(ap_ways[i], QString::number(i));
			relation.add(ap_relations[i], QString::number(i));
		}
		for (int i = 0; i < N_TO_DELETE; ++i) {
			delete ap_nodes[i];
			delete ap_ways[i];
			delete ap_relations[i];
		}
		QCOMPARE(relation.get_size(), 3*N_OBJECTS - 3*N_TO_DELETE);
	}
};

QTEST_MAIN(Test_Osm_Relation)
#include "test_osm_relation.moc"

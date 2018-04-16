#include "osm_elements.h"
#include <QtTest>
using namespace ns_osm;
class Test_Referential_Integrity : public QObject {
	Q_OBJECT
private slots:
	void way_remove_orphaned_nodes() {
		Osm_Way* p_way_1 = new Osm_Way;
		Osm_Way* p_way_2 = new Osm_Way;
		Osm_Node* p_node_1 = new Osm_Node(1.1, 11.1);
		Osm_Node* p_node_2 = new Osm_Node(2.2, 22.2);

		p_way_1->push_node(p_node_1);
//		p_way_1->push_node(p_node_2);
		p_way_2->push_node(p_node_1);
		p_way_2->push_node(p_node_2);
		delete p_way_2;
		QEXPECT_FAIL("", "An orphaned node has been deleted", Continue);
		QCOMPARE(0.0, p_node_2->get_lat());
		QCOMPARE(1.1, p_node_1->get_lat());
	}

	void relation_do_not_remove_orphaned_objects() {
		Osm_Relation* p_rel = new Osm_Relation;
		Osm_Node* p_node = new Osm_Node(1.11, 11.1);

		p_rel->add(p_node, "");
		delete p_rel;
		QCOMPARE(p_node->get_id(), p_node->get_id()); /* No failures happens */
	}
};

QTEST_MAIN(Test_Referential_Integrity)
#include "test_referential_integrity.moc"

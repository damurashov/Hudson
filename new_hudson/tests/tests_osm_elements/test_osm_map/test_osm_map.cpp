#include <QString>
#include <QtTest>
#include "osm_elements.h"
using namespace ns_osm;

class Test_Osm_Map : public QObject
{
	Q_OBJECT
private slots:
	void count_parents() {
		Osm_Map map;
		QCOMPARE(0, map.count_parents());
		map.adopt();
		QCOMPARE(1, map.count_parents());
		map.orphan();
		QCOMPARE(0, map.count_parents());
	}

	void get_node() {
		Osm_Map		map;
		Osm_Node*	p_node = new Osm_Node(1.11,11.1);
		Osm_Node*	p_node2 = new Osm_Node(2.22, 22.2);
		long long	id;

		map.add(p_node);
		map.add(p_node2);
		QCOMPARE(p_node, map.get_node(p_node->get_id()));
		QCOMPARE(p_node2, map.get_node(p_node2->get_id()));

		id = p_node->get_id();
		map.remove(p_node);
		QCOMPARE(nullptr, map.get_node(id));
		QEXPECT_FAIL("", "", Continue);
		QCOMPARE(id, p_node->get_id());
	}

	void get_way() {
		Osm_Map map;
		Osm_Way* p_way1 = new Osm_Way;
		Osm_Way* p_way2 = new Osm_Way;
		long long id;

		map.add(p_way1);
		map.add(p_way2);
		QCOMPARE(p_way1, map.get_way(p_way1->get_id()));
		QCOMPARE(p_way2, map.get_way(p_way2->get_id()));
		id = p_way1->get_id();
		map.remove(p_way1);
		QCOMPARE(nullptr, map.get_way(id));
		QEXPECT_FAIL("","", Continue);
		QCOMPARE(id, p_way1->get_id());
	}

	void get_relation() {
		Osm_Map map;
		Osm_Relation* p_rel1 = new Osm_Relation;
		Osm_Relation* p_rel2 = new Osm_Relation;
		long long id;

		map.add(p_rel1);
		map.add(p_rel2);
		QCOMPARE(p_rel1, map.get_relation(p_rel1->get_id()));
		QCOMPARE(p_rel2, map.get_relation(p_rel2->get_id()));

		id = p_rel1->get_id();
		map.remove(p_rel1);
		QCOMPARE(nullptr, map.get_relation(id));
		QEXPECT_FAIL("","",Continue);
		QCOMPARE(id, p_rel1->get_id());
	}

	void node_iterator() {
		const int	N_NODES = 10;
		Osm_Map		map;
		int			counter = 0;

		for (int i = 0; i < N_NODES; ++i) {
			map.add(new Osm_Node(i,i));
		}
		for (Osm_Map::node_iterator it = map.nbegin(); it != map.nend(); ++it) {
			counter++;
		}
		QCOMPARE(N_NODES, counter);

		counter = 0;
		for (Osm_Map::cnode_iterator it = map.cnbegin(); it != map.cnend(); ++it) {
			counter++;
		}
		QCOMPARE(N_NODES, counter);
	}
};

QTEST_MAIN(Test_Osm_Map)
#include "test_osm_map.moc"

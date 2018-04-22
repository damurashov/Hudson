#include "osm_elements.h"
#include <QtTest>
using namespace ns_osm;

enum Event_Type {
	UNDEFINED,
	NODE_UPDATE,
	WAY_UPDATE,
	RELATION_UPDATE,
	NODE_DELETE,
	WAY_DELETE,
	RELATION_DELETE
};

/*================================================================*/
/*                       Mock subscriber                          */
/*================================================================*/

class Sub : public Osm_Subscriber {
	Event_Type m_last;
public:
	void handle_event_delete(Osm_Node&)		override {m_last = NODE_DELETE;}
	void handle_event_delete(Osm_Way&)		override {m_last = WAY_DELETE; }
	void handle_event_delete(Osm_Relation&) override {m_last = RELATION_DELETE;}
	void handle_event_update(Osm_Node&)		override {m_last = NODE_UPDATE;}
	void handle_event_update(Osm_Way&)		override {m_last = WAY_UPDATE; }
	void handle_event_update(Osm_Relation&) override {m_last = RELATION_UPDATE;}
	Event_Type last() {
		Event_Type temp = m_last;
		m_last = UNDEFINED;
		return temp;
	}
	Sub(Osm_Object& object)	{
		subscribe(object);
		m_last = UNDEFINED;
	}
};

/*================================================================*/
/*                          Test body                             */
/*================================================================*/


class Test_Referential_Integrity : public QObject {
	Q_OBJECT
private slots:
	/* Make sure Sub works in a proper way */
	void sub() {
		Osm_Node node(1.1,1.1);
		Sub sub(node);
		QCOMPARE(sub.last(), UNDEFINED);
		node.set_lat(12.0);
		QCOMPARE(sub.last(), NODE_UPDATE);
		QCOMPARE(sub.last(), UNDEFINED);
	}

	/*================================================================*/
	/*                         Node updates                           */
	/*================================================================*/

	void node_update___sub_listener() {
		Osm_Node node(1.1,1.1);
		Sub sub(node);

		node.set_lat(12.0);
		QCOMPARE(sub.last(), NODE_UPDATE);
		node.set_lon(11.1);
		QCOMPARE(sub.last(), NODE_UPDATE);
		node.set_lat_lon(0.0, 0.0);
		QCOMPARE(sub.last(), NODE_UPDATE);
	}

	void node_update___way_listener() {
		Osm_Node node1(0,0);
		Osm_Node node2(0,0);
		Osm_Way way;

		/* There is 2 ways how to add a node to a way */
		way.push_node(&node1);
		way.insert_node_between(&node2, &node1, &node1);

		Sub sub(way);
		node1.set_lat(12.0);
		node2.set_lat(12.0);
		QCOMPARE(sub.last(), WAY_UPDATE);
		node1.set_lon(11.1);
		node2.set_lon(11.1);
		QCOMPARE(sub.last(), WAY_UPDATE);
		node1.set_lat_lon(0.0, 0.0);
		node2.set_lat_lon(0.0, 0.0);
		QCOMPARE(sub.last(), WAY_UPDATE);
	}

	void node_update___relation_listener() {
		Osm_Node node(0.0, 0.0);
		Osm_Relation rel;

		rel.add(&node);
		Sub sub(rel);
		node.set_lat(12.0);
		QCOMPARE(sub.last(), RELATION_UPDATE);
		node.set_lon(11.1);
		QCOMPARE(sub.last(), RELATION_UPDATE);
		node.set_lat_lon(0.0, 0.0);
		QCOMPARE(sub.last(), RELATION_UPDATE);
	}

	/*================================================================*/
	/*                        Node deletions                          */
	/*================================================================*/

	void node_delete___sub_listener() {
		Osm_Node* p_node = new Osm_Node(0,0);
		Sub sub(*p_node);

		delete p_node;
		QCOMPARE(sub.last(), NODE_DELETE);
	}

	void node_delete___way_listener() {
		Osm_Node* p1_node = new Osm_Node(1.0, 1.0);
		Osm_Node* p2_node = new Osm_Node(2.0, 2.0);
		Osm_Way way;

		way.push_node(p2_node);
		way.insert_node_between(p1_node, p2_node, p2_node);
		Sub sub(way);
		delete p1_node;
		QCOMPARE(sub.last(), WAY_UPDATE);
		delete p2_node;
		QCOMPARE(sub.last(), WAY_UPDATE);
	}

	void node_delete___relation_listener() {
		Osm_Node* p_node = new Osm_Node(1.1,1.1);
		Osm_Relation relation;

		relation.add(p_node);
		Sub sub(relation);
		delete p_node;
		QCOMPARE(sub.last(), RELATION_UPDATE);
	}

	/*================================================================*/
	/*                         Way updates                            */
	/*================================================================*/

	void way_update___sub_listener() {
		Osm_Node node1(0,0);
		Osm_Node node2(0,0);
		Osm_Way way;
		Sub sub(way);

		way.push_node(&node1);
		QCOMPARE(sub.last(), WAY_UPDATE);
		way.insert_node_between(&node2, &node1, &node1);
		QCOMPARE(sub.last(), WAY_UPDATE);
	}

	void way_update___relation_listener() {
		Osm_Node node1(0,0);
		Osm_Node node2(0,0);
		Osm_Way way;
		Osm_Relation relation;

		relation.add(&way);
		Sub sub(relation);
		way.push_node(&node1);
		QCOMPARE(sub.last(), RELATION_UPDATE);
		way.insert_node_between(&node2, &node1, &node1);
		QCOMPARE(sub.last(), RELATION_UPDATE);
	}

	/*================================================================*/
	/*                        Way deletions                           */
	/*================================================================*/

	void way_delete___sub_listener() {
		Osm_Way* p_way = new Osm_Way;
		Sub sub(*p_way);

		delete p_way;
		QCOMPARE(sub.last(), WAY_DELETE);
	}

	void way_delete___relation_listener() {
		Osm_Way* p_way = new Osm_Way;
		Osm_Relation relation;

		relation.add(p_way);
		Sub sub(relation);
		delete p_way;
		QCOMPARE(sub.last(), RELATION_UPDATE);
	}

	/*================================================================*/
	/*                       Relation updates                         */
	/*================================================================*/

	void relation_update___sub_listener() {
		Osm_Node* p_node = new Osm_Node(1,1);
		Osm_Way* p_way = new Osm_Way;
		Osm_Relation* p_rel = new Osm_Relation;
		Osm_Relation rel;
		Sub sub(rel);

		rel.add(p_node);
		QCOMPARE(sub.last(), RELATION_UPDATE);
		rel.add(p_way);
		QCOMPARE(sub.last(), RELATION_UPDATE);
		rel.add(p_rel);
		QCOMPARE(sub.last(), RELATION_UPDATE);

		rel.set_role(p_node, "");
		QCOMPARE(sub.last(), RELATION_UPDATE);
		rel.set_role(p_way, "");
		QCOMPARE(sub.last(), RELATION_UPDATE);
		rel.set_role(p_rel, "");
		QCOMPARE(sub.last(), RELATION_UPDATE);

		rel.remove(p_node);
		QCOMPARE(sub.last(), RELATION_UPDATE);
		rel.remove(p_way);
		QCOMPARE(sub.last(), RELATION_UPDATE);
		rel.remove(p_rel);
		QCOMPARE(sub.last(), RELATION_UPDATE);
	}

	void relation_update___relation_listener() {
		Osm_Node* p_node = new Osm_Node(1,1);
		Osm_Way* p_way = new Osm_Way;
		Osm_Relation* p_rel = new Osm_Relation;
		Osm_Relation rel_source;
		Osm_Relation rel_listener;

		rel_listener.add(&rel_source);
		Sub sub(rel_listener);

		rel_source.add(p_node);
		QCOMPARE(sub.last(), RELATION_UPDATE);
		rel_source.add(p_way);
		QCOMPARE(sub.last(), RELATION_UPDATE);
		rel_source.add(p_rel);
		QCOMPARE(sub.last(), RELATION_UPDATE);

		rel_source.set_role(p_node, "");
		QCOMPARE(sub.last(), RELATION_UPDATE);
		rel_source.set_role(p_way, "");
		QCOMPARE(sub.last(), RELATION_UPDATE);
		rel_source.set_role(p_rel, "");
		QCOMPARE(sub.last(), RELATION_UPDATE);

		rel_source.remove(p_node);
		QCOMPARE(sub.last(), RELATION_UPDATE);
		rel_source.remove(p_way);
		QCOMPARE(sub.last(), RELATION_UPDATE);
		rel_source.remove(p_rel);
		QCOMPARE(sub.last(), RELATION_UPDATE);
	}

	/*================================================================*/
	/*                      Relation deletions                        */
	/*================================================================*/

	void relation_delete___sub_listener() {
		Osm_Relation* p_rel = new Osm_Relation;
		Sub sub(*p_rel);

		delete p_rel;
		QCOMPARE(sub.last(), RELATION_DELETE);
	}

	void relation_delete___relation_listener() {
		Osm_Relation* p_rel = new Osm_Relation;
		Osm_Relation relation;

		relation.add(p_rel);
		Sub sub(relation);
		delete p_rel;
		QCOMPARE(sub.last(), RELATION_UPDATE);
	}

	/*================================================================*/
	/*                           Test map                             */
	/*================================================================*/

	void node_delete___map_listener() {
		Osm_Node* p_node = new Osm_Node(1.1, 1.1);
		Osm_Map map;
		long long id = p_node->get_id();

		map.add(p_node);
		QCOMPARE(true, map.has(p_node));
		Sub sub(*p_node);
		delete p_node;
		QCOMPARE(sub.last(), NODE_DELETE);
		QCOMPARE(nullptr, map.get_node(id));
	}

	void way_delete___map_listener() {
		Osm_Way* p_way = new Osm_Way;
		Osm_Map map;
		long long id = p_way->get_id();

		map.add(p_way);
		QCOMPARE(true, map.has(p_way));
		Sub sub(*p_way);
		delete p_way;
		QCOMPARE(sub.last(), WAY_DELETE);
		QCOMPARE(nullptr, map.get_way(id));
	}

	void relation_delete___map_listener() {
		Osm_Relation* p_relation = new Osm_Relation;
		Osm_Map map;
		long long id = p_relation->get_id();

		map.add(p_relation);
		Sub sub(*p_relation);
		QCOMPARE(true, map.has(p_relation));
		delete p_relation;
		QCOMPARE(sub.last(), RELATION_DELETE);
		QCOMPARE(nullptr, map.get_relation(id));
	}


};

QTEST_MAIN(Test_Referential_Integrity)
#include "test_referential_integrity.moc"

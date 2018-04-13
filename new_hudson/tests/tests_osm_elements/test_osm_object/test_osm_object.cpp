#include "osm_elements.h"
#include <QtTest>
using namespace ns_osm;
class Test_Osm_Object : public QObject
{
	Q_OBJECT
private slots:
	void set_tag() {
		Osm_Node node(1.1, 11.1);
		node.set_tag(QString("this_tag"), QString("this_tag_value"));
	}

	void set_attr() {
		Osm_Way way;
		way.set_attr(QString("this_attr"), QString("this_attr_value"));
	}

	void get_tag_map() {
		Osm_Relation relation;

		QCOMPARE(true, relation.get_tag_map().isEmpty());

		relation.set_tag(QString("this_tag"), QString("this_tag_value"));
		QCOMPARE(true, relation.get_tag_map().contains(QString("this_tag")));
	}

	void get_attr_map() {
		Osm_Node node(1.1, 11.1);

		QCOMPARE(true, node.get_attr_map().contains(QString("id")));

		node.set_attr(QString("id"), QString("someid"));
		QCOMPARE(false, node.get_attr_value(QString("id")) == QString("someid"));
	}

	void get_tag_value() {
		Osm_Way way;

		way.set_tag("tag", "tag");
		QCOMPARE("tag", way.get_tag_value("tag"));
	}

	void get_attr_value() {
		Osm_Relation rel;

		QCOMPARE(false, rel.get_attr_value(QString("id")) == QString(""));
	}

	void get_id() {
		Osm_Node node_1(1.1, 1.1);
		Osm_Node node_2(QString("1234"), "2.2", "2.2");

		QCOMPARE(node_1.get_id(), node_1.get_attr_value("id").toLongLong());
		QCOMPARE(true, node_1.get_id() < 0);
		QCOMPARE(1234, node_2.get_id());
	}

	void remove_tag() {
		Osm_Way way;

		way.set_tag("highway", "service");
		QCOMPARE(true, way.get_tag_map().contains("highway"));

		way.remove_tag("highway");
		QCOMPARE(false, way.get_tag_map().contains("highway"));
	}

	void clear_tags() {
		Osm_Relation rel;

		rel.set_tag("red", "blue");
		rel.set_tag("parents", "mom_and_dad");
		rel.set_tag("bob", "alice");
		QCOMPARE(false, rel.get_tag_map().isEmpty());

		rel.clear_tags();
		QCOMPARE(true, rel.get_tag_map().isEmpty());

	}
};

QTEST_MAIN(Test_Osm_Object)

#include "test_osm_object.moc"

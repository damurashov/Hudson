#include <QtTest>
#include "osm_widget.h"
using namespace ns_osm;
#define COMPARE_ATTR(osm_object, attr_name, exp_value) \
	QCOMPARE(osm_object->get_attr_value(attr_name), QString(#exp_value))

#define COMPARE_TAG(osm_object, tag_name, exp_value) \
	QCOMPARE(osm_object->get_tag_value(QString(#tag_name)), QString(#exp_value))

const char* VISIBLE = "visible";
const char* VERSION = "version";
const char* CHANGESET = "changeset";
const char* TIMESTAMP = "timestamp";
const char* USER = "user";
const char* UID = "uid";
const char* ID = "id";
const char* LAT = "lat";
const char* LON = "lon";

class Test_Osm_Xml : public QObject {
	Q_OBJECT
private slots:
	void load_from_xml() {
		Osm_Widget* p_osmw = new Osm_Widget;
		QCOMPARE(OSM_OK, p_osmw->load_from_xml(PATH_GENUINE_MAP));
	}

	void load_from_xml___bounding_rect() {
		Osm_Widget osmw;

		osmw.load_from_xml(PATH_TEST_MAP);
		QCOMPARE(50.0, osmw.mp_map->m_bounding_rect.left());
		QCOMPARE(10.0, osmw.mp_map->m_bounding_rect.bottom());
		QCOMPARE(90.0, osmw.mp_map->m_bounding_rect.right());
		QCOMPARE(20.0, osmw.mp_map->m_bounding_rect.top());
	}

	void load_from_xml___nodes() {
		Osm_Widget					 osmw;
		Osm_Node*					 p_node;
		QHash<long long, Osm_Node*>& nodes = osmw.mp_map->m_nodes_hash;

		QCOMPARE(OSM_OK, osmw.load_from_xml(PATH_TEST_MAP));
		//QCOMPARE(5, nodes.count());

		/* node 1 */
		p_node = nodes[1];
		QCOMPARE(15.0, p_node->get_lat());
		QCOMPARE(30.0, p_node->get_lon());
		QCOMPARE(1, p_node->get_id());
		QCOMPARE(true, p_node->get_tag_map().isEmpty());
		COMPARE_ATTR(p_node, ID,		1);
		COMPARE_ATTR(p_node, VISIBLE,	true);
		COMPARE_ATTR(p_node, VERSION,	4);
		COMPARE_ATTR(p_node, CHANGESET, 47959292);
		COMPARE_ATTR(p_node, TIMESTAMP, 2017-04-20T05:27:59Z);
		COMPARE_ATTR(p_node, USER,		Danidin9);
		COMPARE_ATTR(p_node, UID,		395588);
		COMPARE_ATTR(p_node, LAT,		15);
		COMPARE_ATTR(p_node, LON,		30);

		/* node 2 */
		p_node = nodes[2];
		QCOMPARE(15.5, p_node->get_lat());
		QCOMPARE(30.5, p_node->get_lon());
		QCOMPARE(2, p_node->get_id());
		QCOMPARE(true, p_node->get_tag_map().isEmpty());
		COMPARE_ATTR(p_node, VISIBLE, true);
		COMPARE_ATTR(p_node, ID,		2);
		COMPARE_ATTR(p_node, VERSION,	4);
		COMPARE_ATTR(p_node, CHANGESET, 10892006);
		COMPARE_ATTR(p_node, TIMESTAMP, 2012-03-06T16:54:50Z);
		COMPARE_ATTR(p_node, USER,		russianin);
		COMPARE_ATTR(p_node, UID,		433058);
		COMPARE_ATTR(p_node, LAT,		15.5);
		COMPARE_ATTR(p_node, LON,		30.5);

		/* node 3 */
		p_node = nodes[3];
		QCOMPARE(18, p_node->get_lat());
		QCOMPARE(34, p_node->get_lon());
		QCOMPARE(3, p_node->get_id());
		QCOMPARE(true, p_node->get_tag_map().isEmpty());
		COMPARE_ATTR(p_node, ID,		3);
		COMPARE_ATTR(p_node, VISIBLE,	true);
		COMPARE_ATTR(p_node, VERSION,	4);
		COMPARE_ATTR(p_node, CHANGESET, 47959292);
		COMPARE_ATTR(p_node, TIMESTAMP, 2017-04-20T05:27:59Z);
		COMPARE_ATTR(p_node, USER,		Danidin9);
		COMPARE_ATTR(p_node, UID,		395588);
		COMPARE_ATTR(p_node, LAT,		18);
		COMPARE_ATTR(p_node, LON,		34);

		/* node -3 */
		p_node = nodes[-3];
		QCOMPARE(18, p_node->get_lat());
		QCOMPARE(31, p_node->get_lon());
		QCOMPARE(-3, p_node->get_id());
		QCOMPARE(true, p_node->get_tag_map().isEmpty());
		COMPARE_ATTR(p_node, ID,		-3);
		COMPARE_ATTR(p_node, VISIBLE,	true);
		COMPARE_ATTR(p_node, VERSION,	4);
		COMPARE_ATTR(p_node, CHANGESET, 47959292);
		COMPARE_ATTR(p_node, TIMESTAMP, 2017-04-20T05:27:59Z);
		COMPARE_ATTR(p_node, USER,		Danidin9);
		COMPARE_ATTR(p_node, UID,		395588);
		COMPARE_ATTR(p_node, LAT,		18);
		COMPARE_ATTR(p_node, LON,		31);

		/* node 4 */
		p_node = nodes[4];
		QCOMPARE(19, p_node->get_lat());
		QCOMPARE(32, p_node->get_lon());
		QCOMPARE(4, p_node->get_id());
		QCOMPARE(false, p_node->get_tag_map().isEmpty());
		COMPARE_ATTR(p_node, ID,		4);
		COMPARE_ATTR(p_node, VISIBLE,	true);
		COMPARE_ATTR(p_node, VERSION,	4);
		COMPARE_ATTR(p_node, CHANGESET,	47959292);
		COMPARE_ATTR(p_node, TIMESTAMP, 2017-04-20T05:27:59Z);
		COMPARE_ATTR(p_node, USER,		Danidin9);
		COMPARE_ATTR(p_node, UID,		395588);
		COMPARE_ATTR(p_node, LAT,		19);
		COMPARE_ATTR(p_node, LON,		32);
		COMPARE_TAG(p_node, addr:housenumber, 17);
		COMPARE_TAG(p_node, addr:street, Торфяная);
		COMPARE_TAG(p_node, addr:suburb, Михайловка);
	}

	void load_from_xml___ways() {
		Osm_Way*					 p_way;
		Osm_Widget					 osmw;
		QHash<long long, Osm_Node*>& nodes = osmw.mp_map->m_nodes_hash;
		QHash<long long, Osm_Way*>&  ways = osmw.mp_map->m_ways_hash;

		QCOMPARE(OSM_OK, osmw.load_from_xml(PATH_TEST_MAP));

		/* way 11 */
		p_way = ways[11];
		QCOMPARE(true, p_way->is_valid());
		QCOMPARE(3, p_way->get_size());
		QCOMPARE(11, p_way->get_id());
		QCOMPARE(p_way->get_nodes_list().at(0), nodes[-3]);
		QCOMPARE(p_way->get_nodes_list().at(1), nodes[2]);
		QCOMPARE(p_way->get_nodes_list().at(2), nodes[1]);
		COMPARE_ATTR(p_way, ID,			11);
		COMPARE_ATTR(p_way, VISIBLE,	true);
		COMPARE_ATTR(p_way, VERSION,	8);
		COMPARE_ATTR(p_way, CHANGESET,	32801302);
		COMPARE_ATTR(p_way, TIMESTAMP,	2015-07-22T12:52:10Z);
		COMPARE_ATTR(p_way, USER,		Danidin9);
		COMPARE_ATTR(p_way, UID,		395588);
		COMPARE_TAG(p_way, addr:suburb, Михайловка);
		COMPARE_TAG(p_way, highway, residential);
		COMPARE_TAG(p_way, name, Каменный);

		/* way 12 */
		p_way = ways[12];
		QCOMPARE(12, p_way->get_id());
		QCOMPARE(false, p_way->is_valid());
		QCOMPARE(2, p_way->get_size());
		QCOMPARE(p_way->get_nodes_list().at(0), nodes[-3]);
		QCOMPARE(p_way->get_nodes_list().at(1), nodes[2]);
		COMPARE_ATTR(p_way, ID,			12);
		COMPARE_ATTR(p_way, VISIBLE,	true);
		COMPARE_ATTR(p_way, VERSION,	8);
		COMPARE_ATTR(p_way, CHANGESET,  32801302);
		COMPARE_ATTR(p_way, TIMESTAMP,	2015-07-22T12:52:10Z);
		COMPARE_ATTR(p_way, USER,		Danidin9);
		COMPARE_ATTR(p_way, UID,		395588);
		COMPARE_TAG(p_way, addr:suburb, Михайловка);
		COMPARE_TAG(p_way, highway, residential);
		COMPARE_TAG(p_way, name, проспект);
	}

	void load_from_xml___relations() {
		Osm_Widget						 osmw;
		QHash<long long, Osm_Node*>&	 nodes = osmw.mp_map->m_nodes_hash;
		QHash<long long, Osm_Way*>&		 ways = osmw.mp_map->m_ways_hash;
		QHash<long long, Osm_Relation*>& relations = osmw.mp_map->m_relations_hash;
		Osm_Relation*					 p_rel;

		QCOMPARE(OSM_OK, osmw.load_from_xml(PATH_TEST_MAP));

		/* relation 101 */
		//QCOMPARE(true, relations.contains(101));
		p_rel = relations[101];
		QCOMPARE(true,		p_rel->is_valid());
		QCOMPARE(3,			p_rel->get_size());
		QCOMPARE(101,		p_rel->get_id());
		QCOMPARE("first",	p_rel->get_role(relations[102]));
		QCOMPARE("haha",	p_rel->get_role(nodes[1]));
		QCOMPARE("foobar",	p_rel->get_role(ways[12]));
		COMPARE_ATTR(p_rel, ID,			101);
		COMPARE_ATTR(p_rel, VISIBLE,	true);
		COMPARE_ATTR(p_rel, VERSION,	49);
		COMPARE_ATTR(p_rel, CHANGESET,	51446240);
		COMPARE_ATTR(p_rel, TIMESTAMP,	2017-08-25T21:59:20Z);
		COMPARE_ATTR(p_rel, USER,		Dinamik);
		COMPARE_ATTR(p_rel, UID,		39040);
		COMPARE_TAG(p_rel,	name,		Каменный);

		/* relation 102 */
		p_rel = relations[102];
		QCOMPARE(false,		p_rel->is_valid());
		QCOMPARE(2,			p_rel->get_size());
		QCOMPARE(102,		p_rel->get_id());
		QCOMPARE("none",	p_rel->get_role(nodes[1]));
		QCOMPARE("none",	p_rel->get_role(ways[11]));
		COMPARE_ATTR(p_rel,	ID,			102);
		COMPARE_ATTR(p_rel,	VISIBLE,	true);
		COMPARE_ATTR(p_rel,	VERSION,	49);
		COMPARE_ATTR(p_rel, CHANGESET,	51446240);
		COMPARE_ATTR(p_rel, TIMESTAMP,	2017-08-25T21:59:20Z);
		COMPARE_ATTR(p_rel, USER,		Dinamik);
		COMPARE_ATTR(p_rel, UID,		39040);
	}
};

QTEST_MAIN(Test_Osm_Xml)
#include "test_osm_xml.moc"

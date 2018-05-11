#include <QtTest>
#include "osm_elements.h"

using namespace ns_osm;

class Test_Osm_Map_Bound : public QObject {
	Q_OBJECT
private slots:
	void includes_in_scene___issue180___default_rect() {
		Osm_Map map;
		Osm_Node node_in(10.123456, 170.99165);
		Osm_Node node_in_2(-10.0, -173.1234);
		Osm_Node node_in_3(0, -160);
		Osm_Node node_out(-4.0, -143.76889);
		QRectF rect;

		rect.setLeft(160);
		rect.setRight(-160);
		rect.setTop(20);
		rect.setBottom(-20);
		map.set_bound(rect);

		QCOMPARE(true, map.includes_in_scene(&node_in));
		QCOMPARE(true, map.includes_in_scene(&node_in_2));
		QCOMPARE(true, map.includes_in_scene(&node_in_3));
		QCOMPARE(false, map.includes_in_scene(&node_out));
	}

	void includes_in_scene___no_issue180___default_rect() {
		Osm_Map map;
		QRectF rect;
		Osm_Node node_in(0,0);
		Osm_Node node_in_2(20,20);
		Osm_Node node_out(-30, -30);

		rect.setLeft(-20);
		rect.setBottom(-20);
		rect.setRight(20);
		rect.setTop(20);
		map.set_bound(rect);

		QCOMPARE(true, map.includes_in_scene(&node_in));
		QCOMPARE(true, map.includes_in_scene(&node_in_2));
		QCOMPARE(false, map.includes_in_scene(&node_out));
	}

	void includes_in_scene___issue180___autorect() {
		Osm_Map map;
		QRectF bound;
		QRectF scene;
		QRectF reference_bound;
		QRectF reference_scene;
		Osm_Node left(0.0, 175.0);
		Osm_Node center(0.0, -180);
		Osm_Node right(10.0, -160.0);

		map.set_remove_physically(false);
		map.add(&left);
		map.add(&right);
		map.add(&center);
		QCOMPARE(true, map.includes_in_scene(&left));
		QCOMPARE(true, map.includes_in_scene(&right));
		QCOMPARE(true, map.has(&center));

		bound = map.get_bound();
		scene = map.get_scene_rect();

		reference_bound.setLeft(175.0);
		reference_bound.setBottom(0.0);
		reference_bound.setRight(-160.0);
		reference_bound.setTop(10.0);
		reference_scene.setLeft(175.0);
		reference_scene.setBottom(0.0);
		reference_scene.setRight((-160.0+360.0));
		reference_scene.setTop(10.0);

		QCOMPARE(reference_scene.left(), scene.left());
		QCOMPARE(reference_scene.right(), scene.right());
		QCOMPARE(reference_scene.top(), scene.top());
		QCOMPARE(reference_scene.bottom(), scene.bottom());

		QCOMPARE(reference_bound.left(), bound.left());
		QCOMPARE(reference_bound.right(), bound.right());
		QCOMPARE(reference_bound.top(), bound.top());
		QCOMPARE(reference_bound.bottom(), bound.bottom());
	}

	void includes_in_scene___no_issue180___autorect() {
		Osm_Map map;
		QRectF bound;
		QRectF scene;
		Osm_Node left(-10,-10);
		Osm_Node right(10, 10);

		map.set_remove_physically(false);
		map.add(&left);
		map.add(&right);

		bound = map.get_bound();
		scene = map.get_scene_rect();
		scene.setBottomLeft(map.get_geo_coord(scene.bottomLeft()));
		scene.setTopRight(map.get_geo_coord(scene.topRight()));
		QCOMPARE(bound, scene);
		QCOMPARE(-10.0, bound.left());
		QCOMPARE(10.0, bound.right());
	}

	void get_scene_coord___issue180() {
		Osm_Map map;
		QRectF rect;
		Osm_Node node(0, -170.0);

		rect.setLeft(160.0);
		rect.setRight(-160.0);
		rect.setTop(10);
		rect.setBottom(-10);
		map.set_bound(rect);

		QCOMPARE(map.get_scene_coord(&node).y(), 0);
		QCOMPARE(map.get_scene_coord(&node).x(), (-170.0 + 360.0));

		QCOMPARE((-160.0+360.0), map.get_scene_rect().right());
	}
};

QTEST_MAIN(Test_Osm_Map_Bound)
#include "test_osm_map_bound.moc"

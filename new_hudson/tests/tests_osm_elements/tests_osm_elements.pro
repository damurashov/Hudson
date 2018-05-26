TEMPLATE = subdirs

SUBDIRS += \
    test_osm_node \
    test_osm_way \
    test_osm_relation \
    test_referential_integrity \
    test_osm_object \
    test_osm_map \

test_osm_node.subdirs = test_osm_node
test_osm_way.subdirs = test_osm_way
test_osm_relation.subdirs = test_osm_relation

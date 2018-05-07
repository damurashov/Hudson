TEMPLATE = subdirs

#CONFIG += ordered

SUBDIRS += \
    osm_elements    \
    osm_widget      \
    tests

osm_widget.depends = osm_elements
tests.depends = osm_elements
tests.depends = osm_widget

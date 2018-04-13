TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += \
    osm_elements \
    tests \
    osm_widget

tests.depends = osm_widget
osm_widget.depends = osm_elements

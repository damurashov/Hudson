TEMPLATE = subdirs

CONFIG += ordered

QT += testlib core

SUBDIRS += \
    tests_osm_elements \
    #test_osm_widget

#tests_osm_elements.subdir = tests_osm_elements
#test_osm_widget.subdir = test_osm_widget

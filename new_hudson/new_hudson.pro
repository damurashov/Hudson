TEMPLATE = subdirs

#CONFIG += ordered

QMAKE_CXXFLAGS += \
-Wno-unused-parameter   \
-Wno-ignored-qualifiers

SUBDIRS += \
    osm_elements    \
    osm_widget      \
    tests \
    hudson_app

osm_widget.depends = osm_elements
tests.depends = osm_elements
tests.depends = osm_widget

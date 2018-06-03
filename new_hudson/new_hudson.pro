TEMPLATE = subdirs

#CONFIG += ordered

QMAKE_CXXFLAGS += \
-Wno-unused-parameter   \
-Wno-ignored-qualifiers

SUBDIRS += \
    osm_elements    \
    osm_widget      \
    hudson_app
#    tests \

osm_widget.depends = osm_elements
tests.depends = osm_elements
tests.depends = osm_widget

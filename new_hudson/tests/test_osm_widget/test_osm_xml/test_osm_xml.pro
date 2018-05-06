TEMPLATE = app

QT += testlib widgets core xml

CONFIG += c++11 #link_prl

DEFINES += \
    PATH_GENUINE_MAP=\\\"$$PWD/../map.osm\\\"  \
    PATH_TEST_MAP=\\\"$$PWD/../test_map.osm\\\" \
    private=public \
    protected=public

INCLUDEPATH += \
    $$PWD/../../../osm_elements             \
    $$PWD/../../../osm_widget/              \
    $$PWD/../../../osm_widget/xml_handler   \
    $$PWD/../../../osm_widget/view_handler

LIBS += -L$$PWD/../../../intermediate_libs/ -losm_widget
LIBS += -L$$PWD/../../../intermediate_libs/ -losm_elements

SOURCES += \
    test_osm_xml.cpp


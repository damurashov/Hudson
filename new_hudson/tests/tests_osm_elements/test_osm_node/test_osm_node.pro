TEMPLATE = app

QT += testlib core

CONFIG += \
#    link_prl \
    c++11

INCLUDEPATH = $$PWD/../../../osm_elements/

#SRC_PREF = $$PWD/../../../osm_elements/
LIBS += -L$$PWD/../../../intermediate_libs/ -losm_elements

SOURCES += \
    test_osm_node.cpp

TEMPLATE = app

CONFIG += c++11

QT += core testlib

INCLUDEPATH += $$PWD/../../../osm_elements

LIBS += -L$$PWD/../../../intermediate_libs -losm_elements

DEFINES += private=public\
    protected=public

SOURCES += \
    test_osm_map_bound.cpp

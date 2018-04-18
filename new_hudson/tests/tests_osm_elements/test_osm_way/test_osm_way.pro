QT += testlib core

DEFINES = private=public\
    protected=public

TEMPLATE = app

CONFIG += c++11

INCLUDEPATH = $$PWD/../../../osm_elements

LIBS += -L$$PWD/../../../intermediate_libs/ -losm_elements

SOURCES += \
    test_osm_way.cpp

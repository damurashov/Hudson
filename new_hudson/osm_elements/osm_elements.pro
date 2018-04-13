TEMPLATE = lib

QT += core

TARGET = osm_elements

CONFIG += c++11 \
    staticlib\
    #create_prl\
    #link_prl

DESTDIR = $$PWD/../intermediate_libs/

SOURCES += \
        osm_object.cpp \
        osm_node.cpp \
        osm_way.cpp \
        osm_relation.cpp

HEADERS += \
        osm_elements.h \
        osm_node.h \
        osm_way.h \
        osm_relation.h \
        osm_object.h \

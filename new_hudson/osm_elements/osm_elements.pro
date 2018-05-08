TEMPLATE = lib

QT += core

QMAKE_CXXFLAGS += \
-Wno-unused-parameter   \
-Wno-ignored-qualifiers

TARGET = osm_elements

CONFIG += c++11 \
    staticlib\

DESTDIR = $$PWD/../intermediate_libs/

SOURCES += \
        osm_object.cpp \
        osm_node.cpp \
        osm_way.cpp \
        osm_relation.cpp \
    osm_map.cpp \
    osm_subscriber.cpp \
    osm_info.cpp \
    meta.cpp

HEADERS += \
        osm_elements.h \
        osm_node.h \
        osm_way.h \
        osm_relation.h \
        osm_object.h \
    osm_map.h \
    osm_subscriber.h \
    osm_info.h \
    meta.h

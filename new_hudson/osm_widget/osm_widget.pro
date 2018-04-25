TEMPLATE = lib

DESTDIR = $$PWD/../intermediate_libs

TARGET = osm_widget

QT += core widgets xml

CONFIG += c++11\
    staticlib\
    #create_prl\
    #link_prl

INCLUDEPATH = $$PWD/../osm_elements/

LIBS += -L$$PWD/../intermediate_libs/ -losm_elements

SOURCES += \
    osm_widget.cpp \
    osm_xml.cpp \
    xml_handler.cpp \
    osm_message.cpp \
    view_handler.cpp \
    osm_message.cpp \
    osm_widget.cpp \
    osm_xml.cpp \
    view_handler.cpp \
    xml_handler.cpp \
    items/item_node.cpp
    #item_node.cpp \
    #item_edge.cpp \

HEADERS += \
    osm_widget.h \
    osm_xml.h \
    #item_node.h \
    #item_edge.h \
    xml_handler.h \
    osm_message.h \
    view_handler.h \
    osm_message.h \
    osm_widget.h \
    osm_xml.h \
    view_handler.h \
    xml_handler.h \
    items/item_node.h

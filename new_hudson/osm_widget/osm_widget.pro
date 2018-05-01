TEMPLATE = lib

DESTDIR = $$PWD/../intermediate_libs

TARGET = osm_widget

QT += core widgets xml

CONFIG += c++11\
    staticlib\
    #create_prl\
    #link_prl

INCLUDEPATH = $$PWD/../osm_elements/ \
    $$PWD/view_handler \
    $$PWD/xml_handler

LIBS += -L$$PWD/../intermediate_libs/ -losm_elements

SOURCES += \
    xml_handler.cpp \
    xml_handler/osm_xml.cpp \
    xml_handler/xml_handler.cpp \
    view_handler/view_handler.cpp \
    view_handler.cpp \
    view_handler/item_node.cpp \
    osm_widget.cpp \
    osm_xml.cpp \
    osm_message.cpp \
    osm_message.cpp \
    osm_widget.cpp \
    view_handler/item_edge.cpp \
    view_handler/item_way.cpp \
    view_handler/osm_view.cpp \
    view_handler/edge.cpp

HEADERS += \
    xml_handler.h \
    xml_handler/osm_xml.h \
    xml_handler/xml_handler.h \
    view_handler/view_handler.h \
    view_handler.h \
    view_handler/item_node.h\
    osm_widget.h \
    osm_message.h \
    osm_message.h \
    osm_widget.h \
    view_handler/item_edge.h \
    view_handler/item_way.h \
    view_handler/osm_view.h \
    view_handler/edge.h

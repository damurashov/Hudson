TEMPLATE = lib

QMAKE_CXXFLAGS += \
-Wno-unused-parameter   \
-Wno-ignored-qualifiers

DESTDIR = $$PWD/../intermediate_libs

TARGET = osm_widget

QT += gui core widgets xml

CONFIG += c++11\
    staticlib\
    #create_prl\
    #link_prl

INCLUDEPATH = $$PWD/../osm_elements/    \
    $$PWD/view_handler/                 \
    $$PWD/xml_handler/                  \
    $$PWD/info_widget/

LIBS += -L$$PWD/../intermediate_libs/ -losm_elements

SOURCES += \
osm_message.cpp                 \
osm_widget.cpp                  \
view_handler/edge.cpp           \
view_handler/item_edge.cpp      \
view_handler/item_node.cpp      \
view_handler/item_way.cpp       \
view_handler/osm_view.cpp       \
view_handler/view_handler.cpp   \
xml_handler/osm_xml.cpp         \
xml_handler/xml_handler.cpp     \
info_widget/tag_table.cpp       \
info_widget/info_widget.cpp     \

HEADERS += \
osm_message.h                   \
osm_widget.h                    \
view_handler/edge.h             \
view_handler/item_edge.h        \
view_handler/item_node.h        \
view_handler/item_way.h         \
view_handler/osm_view.h         \
view_handler/view_handler.h     \
xml_handler/osm_xml.h           \
xml_handler/xml_handler.h       \
info_widget/info_widget.h       \
info_widget/tag_table.h         \

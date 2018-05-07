TEMPLATE = app

QT += gui core widgets xml

INCLUDEPATH += \
$$PWD/../../../osm_widget \
$$PWD/../../../osm_elements

DEFINES += \
    PATH_GENUINE_MAP=\\\"$$PWD/../map.osm\\\"  \
    PATH_TEST_MAP=\\\"$$PWD/../test_map.osm\\\" \
    private=public \
    protected=public

LIBS += -L$$PWD/../../../intermediate_libs/ -losm_widget
LIBS += -L$$PWD/../../../intermediate_libs/ -losm_elements

CONFIG += c++11

SOURCES += \
    main.cpp

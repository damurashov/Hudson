TEMPLATE = app

QT += gui core widgets xml

INCLUDEPATH += \
$$PWD/../../../osm_widget   \
$$PWD/../../../osm_elements \
$$PWD/../../../hudson_app   \

DEFINES += \
    PATH_GENUINE_MAP=\\\"$$PWD/../map.osm\\\"           \
    PATH_MAP2=\\\"$$PWD/../map2.osm\\\"                 \
    PATH_TEST_MAP=\\\"$$PWD/../test_map.osm\\\"         \
    PATH_MERKAARTOR_MAP=\\\"$$PWD/../merkaartor.osm\\\" \
    private=public                                      \
    protected=public

LIBS += -L$$PWD/../../../intermediate_libs/ -losm_widget
LIBS += -L$$PWD/../../../intermediate_libs/ -losm_elements

CONFIG += c++11

HEADERS += \
$$PWD/../../../hudson_app/hudson_app.h\

SOURCES += \
    main.cpp \
    $$PWD/../../../hudson_app/hudson_app.cpp\

TEMPLATE = app

QT += gui core widgets xml

INCLUDEPATH += \
$$PWD/../osm_widget \
$$PWD/../osm_elements

LIBS += -L$$PWD/../intermediate_libs/ -losm_widget
LIBS += -L$$PWD/../intermediate_libs/ -losm_elements

CONFIG += c++11

HEADERS += \
    hudson_app.h

SOURCES += \
    hudson_app.cpp \
    main.cpp

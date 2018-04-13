#-------------------------------------------------
#
# Project created by QtCreator 2018-01-27T17:48:42
#
#-------------------------------------------------

QT       += testlib core

CONFIG   += c++11

TEMPLATE = app

INCLUDEPATH = $$PWD/../../../osm_elements

LIBS += -L$$PWD/../../../intermediate_libs/ -losm_elements

SOURCES += \
        test_osm_object.cpp 

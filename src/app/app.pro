TEMPLATE = app
SOURCES = main.cpp \
    controller.cpp \
    filesmodel.cpp \
    worker.cpp
LIBS += -L../image/debug -limage
TARGET = ../app-exe

INCLUDEPATH += ../image

QT += quick

HEADERS += \
    controller.h \
    filesmodel.h \
    worker.h

SOURCES += \
        main.cpp

RESOURCES += resources/qml.qrc


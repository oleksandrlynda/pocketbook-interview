TEMPLATE = app
SOURCES = main.cpp \
    controller.cpp \
    filesmodel.cpp \
    test.cpp \
    worker.cpp
LIBS += -L../image/debug -limage
TARGET = ../app-exe

INCLUDEPATH += ../image

QT += quick

HEADERS += \
    controller.h \
    filesmodel.h \
    test.h \
    worker.h

SOURCES += \
        main.cpp

RESOURCES += resources/qml.qrc


IMAGES_IN=$$absolute_path($$PWD/../../images)
IMAGES_OUT=$$OUT_PWD/images
win32 {
    IMAGES_IN~= s,/,\\,g
    IMAGES_OUT~= s,/,\\,g
}
QMAKE_POST_LINK += $$QMAKE_CHK_DIR_EXISTS $$IMAGES_OUT $$QMAKE_MKDIR $$IMAGES_OUT$$escape_expand(\n\t)
QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$IMAGES_IN $$IMAGES_OUT$$escape_expand(\n\t)

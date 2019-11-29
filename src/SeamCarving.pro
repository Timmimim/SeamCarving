#-------------------------------------------------
#
# Project created by QtCreator 2016-10-16T14:52:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SeamCarving
TEMPLATE = app


SOURCES += main.cpp\
        EnergyFunctions.cpp \
        MainWindow.cpp \
        ImageReader.cpp \
        QtOpencvCore.cpp \
    Seams.cpp

HEADERS  += MainWindow.hpp \
        EnergyFunctions.hpp \
        ImageReader.hpp \
        QtOpencvCore.hpp \
    Seams.h

FORMS    +=

macx {

    # MAC Compiler Flags
}

win32 {
    # Windows Compiler Flags
}

unix {

    QMAKE_CXXFLAGS += -std=c++17 -Wall -pedantic -Wno-unknown-pragmas

    INCLUDEPATH += /usr/include/opencv4 /usr/include/opencv4/opencv2 /usr/local/include/opencv4 /usr/local/include/opencv4/opencv2

    LIBS += -L/usr/local/lib \
            -lopencv_core \
            -lopencv_highgui \
            -lopencv_imgproc \
            -lopencv_imgcodecs

    QMAKE_CXXFLAGS_WARN_ON = -Wno-unused-variable -Wno-reorder
}

#-------------------------------------------------
#
# Project created by QtCreator 2014-05-14T10:15:44
#
#-------------------------------------------------

QT += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Alarm
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    widget/time_widget.cpp \
    widget/jlabel.cpp \
    widget/sound_options.cpp \
    utils/properties.cpp \
    utils/files.cpp \
    utils/strings.cpp \
    widget/chrono_widget.cpp \
    widget/clock_widget.cpp

HEADERS  += mainwindow.hpp \
    widget/time_widget.hpp \
    widget/jlabel.hpp \
    widget/sound_options.hpp \
    utils/properties.hpp \
    utils/files.hpp \
    utils/strings.hpp \
    widget/chrono_widget.hpp \
    widget/clock_widget.hpp

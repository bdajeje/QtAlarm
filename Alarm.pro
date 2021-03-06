#-------------------------------------------------
#
# Project created by QtCreator 2014-05-14T10:15:44
#
#-------------------------------------------------

QT += core gui multimedia widgets

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Alarm
TEMPLATE = app

QMAKE_CXXFLAGS += -std=gnu++1z

INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt5/

SOURCES += main.cpp\
        mainwindow.cpp \
    widget/time_widget.cpp \
    widget/jlabel.cpp \
    widget/sound_options.cpp \
    utils/properties.cpp \
    utils/files.cpp \
    utils/strings.cpp \
    widget/chrono_widget.cpp \
    widget/clock_widget.cpp \
    utils/math.cpp \
    widget/snooze_options.cpp

HEADERS  += mainwindow.hpp \
    widget/time_widget.hpp \
    widget/jlabel.hpp \
    widget/sound_options.hpp \
    utils/properties.hpp \
    utils/files.hpp \
    utils/strings.hpp \
    widget/chrono_widget.hpp \
    widget/clock_widget.hpp \
    utils/math.hpp \
    utils/resources_manager.hpp \
    utils/icons_manager.hpp \
    widget/snooze_options.hpp

TRANSLATIONS = Alarm_en.ts Alarm_fr.ts

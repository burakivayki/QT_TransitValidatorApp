QT       += core gui
QT += serialport
QT += core
audio_files.files = $$PWD/akbil.mp3 $$PWD/akbil.wav
audio_files.path = /home/root
INSTALLS += audio_files

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = QT_TransitValidatorApp

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainpage.cpp \
    mainwindow.cpp \
    qrscanform.cpp \
    scancardpage.cpp \
    soundtestpage.cpp \
    touchscreentest.cpp

HEADERS += \
    mainpage.h \
    mainwindow.h \
    qrscanform.h \
    scancardpage.h \
    soundtestpage.h \
    touchscreentest.h

FORMS += \
    mainpage.ui \
    mainwindow.ui \
    qrscanform.ui \
    scancardpage.ui \
    soundtestpage.ui \
    touchscreentest.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

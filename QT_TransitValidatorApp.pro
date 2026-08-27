QT       += core gui
QT += serialport
QT += core
audio_files.files = $$PWD/assets/akbil.mp3 $$PWD/assets/akbil.wav
audio_files.path = /home/root
INSTALLS += audio_files

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = QT_TransitValidatorApp

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/main.cpp \
    src/mainpage.cpp \
    src/mainwindow.cpp \
    src/qrscanform.cpp \
    src/scancardpage.cpp \
    src/serialprocessor.cpp \
    src/soundtestpage.cpp \
    src/touchscreentest.cpp

HEADERS += \
    src/mainpage.h \
    src/mainwindow.h \
    src/qrscanform.h \
    src/scancardpage.h \
    src/serialprocessor.h \
    src/soundtestpage.h \
    src/touchscreentest.h

FORMS += \
    ui/mainpage.ui \
    ui/mainwindow.ui \
    ui/qrscanform.ui \
    ui/scancardpage.ui \
    ui/soundtestpage.ui \
    ui/touchscreentest.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    rsc.qrc

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

ICON = myaddico.ico

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    packetprocessor.cpp \
    packetsender.cpp \
    settings.cpp \
    uicontroller.cpp \
    userinputdata.cpp

HEADERS += \
    mainwindow.h \
    packetprocessor.h \
    packetsender.h \
    settings.h \
    uicontroller.h \
    userinputdata.h

FORMS += \
    mainwindow.ui \
    settings.ui


DEFINES += HAVE_SNPRINTF
DEFINES += HAVE_VSNPRINTF
INCLUDEPATH += libs/Include
LIBS += wpcap.lib
LIBS += Packet.lib
LIBS += -lIphlpapi
LIBS += -lWs2_32

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia

CONFIG += c++17
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    coverdownloader.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    api_helpers.h \
    coverdownloader.h \
    httplib.h \
    json.hpp \
    json_fwd.hpp \
    logindialog.h \
    mainwindow.h \
    password_crypt.h \
    structures.h

FORMS += \
    logindialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    darkstyle.qss

RESOURCES += \
    darkstyle.qrc
LIBS += -lws2_32 -lcrypt32

# include your openssl and cryptopp paths here

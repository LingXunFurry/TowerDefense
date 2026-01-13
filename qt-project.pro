QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/enemy.cpp \
    src/gameentity.cpp \
    src/gamepage.cpp \
    src/main.cpp \
    src/mainmenupage.cpp \
    src/mainwindow.cpp \
    src/resourcemanager.cpp \
    src/tower.cpp \
    src/bullet.cpp

HEADERS += \
    include/config.h \
    include/enemy.h \
    include/gameentity.h \
    include/gamepage.h \
    include/mainmenupage.h \
    include/mainwindow.h \
    include/resourcemanager.h \
    include/tower.h \
    include/bullet.h

FORMS += \
    ui/mainwindow.ui

RESOURCES += \
    res/res.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

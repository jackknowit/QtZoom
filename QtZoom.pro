QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# 添加 Qt 头文件路径
INCLUDEPATH += /opt/homebrew/opt/qt/include
INCLUDEPATH += /opt/homebrew/opt/qt/lib/QtCore.framework/Headers
INCLUDEPATH += /opt/homebrew/opt/qt/lib/QtGui.framework/Headers
INCLUDEPATH += /opt/homebrew/opt/qt/lib/QtWidgets.framework/Headers

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    drawingboard.cpp

HEADERS += \
    mainwindow.h \
    drawingboard.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target 
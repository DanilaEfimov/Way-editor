QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Error.cpp \
    Graphs/BiTree.cpp \
    Graphs/DPseudoGraph.cpp \
    Graphs/DirGraph.cpp \
    Graphs/Graph.cpp \
    Graphs/Tree.cpp \
    Graphs/UDirGraph.cpp \
    Graphs/UDWGraph.cpp \
    Graphs/UPseudoGraph.cpp \
    Graphs/WDGraph.cpp \
    Graphs/WTree.cpp \
    Parser.cpp \
    PerformanceManager.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Error.h \
    General.h \
    Graphs/BiTree.h \
    Graphs/DPseudoGraph.h \
    Graphs/DirGraph.h \
    Graphs/Graph.h \
    Graphs/Tree.h \
    Graphs/UDirGraph.h \
    Graphs/UDWGraph.h \
    Graphs/UPseudoGraph.h \
    Graphs/WDGraph.h \
    Graphs/WTree.h \
    Parser.h \
    PerformanceManager.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    README.txt \
    build/Desktop_Qt_6_7_2_MinGW_64_bit-Debug/help.txt \
    build/Desktop_Qt_6_7_2_MinGW_64_bit-Debug/hystory.txt \
    way_editor_icon.png

#-------------------------------------------------
#
# Project created by QtCreator 2014-09-21T13:34:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = clang-format-gui
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
    Ui/MainWindow.cpp \
    Ui/ClangFormatCmdSet.cpp \
    SrcFilePreviewer.cpp \
    ClangFormatter.cpp \
    FormatOptions.cpp \
    Utility.cpp \
    Settings.cpp \
    SrcUpdater.cpp \
    DetailsUiControl.cpp \
    DetailsUiControls/StylesUiControl.cpp \
    DetailsUiControls/TabsUiControl.cpp \
    CFConfiguration.cpp \
    DetailsUiControls/IndentUiControl.cpp

HEADERS  += \
    Ui/MainWindow.h \
    Ui/ClangFormatCmdSet.h \
    SrcFilePreviewer.h \
    ClangFormatter.h \
    FormatOptions.h \
    Settings.h \
    SrcUpdater.h \
    DetailsUiControl.h \
    DetailsUiControls/StylesUiControl.h \
    DetailsUiControls/TabsUiControl.h \
    CFConfiguration.h \
    DetailsUiControls/IndentUiControl.h

FORMS    += \
    Ui/ClangFormatCmdSet.ui \
    Ui/Mainwindow.ui

# add qscintilla library for windows
# remember to copy the debug dll of qscintilla to the library path
win32 {
    LIBS += -L"C:\Qt\Qt5.3.0\5.3\mingw482_32\bin"
    LIBS += -lqscintilla2
}

unix {
    LIBS += -L/usr/lib -lqscintilla2
    CONFIG += qscintilla2
}

OTHER_FILES +=

RESOURCES += Resources.qrc

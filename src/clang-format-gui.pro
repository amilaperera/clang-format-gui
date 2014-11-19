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
    DetailsUiControls/TabsUiControl.cpp

HEADERS  += \
    Qsci/qsciabstractapis.h \
    Qsci/qsciapis.h \
    Qsci/qscicommand.h \
    Qsci/qscicommandset.h \
    Qsci/qscidocument.h \
    Qsci/qsciglobal.h \
    Qsci/qscilexer.h \
    Qsci/qscilexeravs.h \
    Qsci/qscilexerbash.h \
    Qsci/qscilexerbatch.h \
    Qsci/qscilexercmake.h \
    Qsci/qscilexercoffeescript.h \
    Qsci/qscilexercpp.h \
    Qsci/qscilexercsharp.h \
    Qsci/qscilexercss.h \
    Qsci/qscilexercustom.h \
    Qsci/qscilexerd.h \
    Qsci/qscilexerdiff.h \
    Qsci/qscilexerfortran.h \
    Qsci/qscilexerfortran77.h \
    Qsci/qscilexerhtml.h \
    Qsci/qscilexeridl.h \
    Qsci/qscilexerjava.h \
    Qsci/qscilexerjavascript.h \
    Qsci/qscilexerlua.h \
    Qsci/qscilexermakefile.h \
    Qsci/qscilexermatlab.h \
    Qsci/qscilexeroctave.h \
    Qsci/qscilexerpascal.h \
    Qsci/qscilexerperl.h \
    Qsci/qscilexerpo.h \
    Qsci/qscilexerpostscript.h \
    Qsci/qscilexerpov.h \
    Qsci/qscilexerproperties.h \
    Qsci/qscilexerpython.h \
    Qsci/qscilexerruby.h \
    Qsci/qscilexerspice.h \
    Qsci/qscilexersql.h \
    Qsci/qscilexertcl.h \
    Qsci/qscilexertex.h \
    Qsci/qscilexerverilog.h \
    Qsci/qscilexervhdl.h \
    Qsci/qscilexerxml.h \
    Qsci/qscilexeryaml.h \
    Qsci/qscimacro.h \
    Qsci/qsciprinter.h \
    Qsci/qsciscintilla.h \
    Qsci/qsciscintillabase.h \
    Qsci/qscistyle.h \
    Qsci/qscistyledtext.h \
    Ui/MainWindow.h \
    Ui/ClangFormatCmdSet.h \
    SrcFilePreviewer.h \
    ClangFormatter.h \
    FormatOptions.h \
    Settings.h \
    SrcUpdater.h \
    DetailsUiControl.h \
    DetailsUiControls/StylesUiControl.h \
    DetailsUiControls/TabsUiControl.h

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
    LIBS += -L/usr/lib/x86_64-linux-gnu -lqscintilla2
    CONFIG += qscintilla2
}

OTHER_FILES +=

RESOURCES += Resources.qrc

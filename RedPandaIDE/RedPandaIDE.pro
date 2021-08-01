QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    HighlighterManager.cpp \
    colorscheme.cpp \
    compiler/compiler.cpp \
    compiler/compilermanager.cpp \
    compiler/executablerunner.cpp \
    compiler/filecompiler.cpp \
    compiler/stdincompiler.cpp \
    settingsdialog/debuggeneralwidget.cpp \
    widgets/cpudialog.cpp \
    debugger.cpp \
    editor.cpp \
    editorlist.cpp \
    iconsmanager.cpp \
    main.cpp \
    mainwindow.cpp \
    qsynedit/CodeFolding.cpp \
    qsynedit/Constants.cpp \
    qsynedit/KeyStrokes.cpp \
    qsynedit/MiscClasses.cpp \
    qsynedit/MiscProcs.cpp \
    qsynedit/SynEdit.cpp \
    qsynedit/TextBuffer.cpp \
    qsynedit/TextPainter.cpp \
    qsynedit/exporter/synexporter.cpp \
    qsynedit/exporter/synhtmlexporter.cpp \
    qsynedit/exporter/synrtfexporter.cpp \
    qsynedit/highlighter/asm.cpp \
    qsynedit/highlighter/base.cpp \
    qsynedit/highlighter/composition.cpp \
    qsynedit/highlighter/cpp.cpp \
    settingsdialog/compilersetdirectorieswidget.cpp \
    settingsdialog/compilersetoptionwidget.cpp \
    settings.cpp \
    settingsdialog/editorclipboardwidget.cpp \
    settingsdialog/editorcolorschemewidget.cpp \
    settingsdialog/editorfontwidget.cpp \
    settingsdialog/editorgeneralwidget.cpp \
    settingsdialog/editorsymbolcompletionwidget.cpp \
    settingsdialog/editorsyntaxcheckwidget.cpp \
    settingsdialog/environmentappearencewidget.cpp \
    settingsdialog/executorgeneralwidget.cpp \
    settingsdialog/settingsdialog.cpp \
    settingsdialog/settingswidget.cpp \
    systemconsts.cpp \
    utils.cpp \
    widgets/coloredit.cpp \
    widgets/consolewidget.cpp \
    widgets/issuestable.cpp \
    widgets/qconsole.cpp \
    widgets/qpatchedcombobox.cpp

HEADERS += \
    HighlighterManager.h \
    colorscheme.h \
    compiler/compiler.h \
    compiler/compilermanager.h \
    compiler/executablerunner.h \
    compiler/filecompiler.h \
    compiler/stdincompiler.h \
    settingsdialog/debuggeneralwidget.h \
    widgets/cpudialog.h \
    debugger.h \
    editor.h \
    editorlist.h \
    iconsmanager.h \
    mainwindow.h \
    qsynedit/CodeFolding.h \
    qsynedit/Constants.h \
    qsynedit/KeyStrokes.h \
    qsynedit/MiscClasses.h \
    qsynedit/MiscProcs.h \
    qsynedit/SynEdit.h \
    qsynedit/TextBuffer.h \
    qsynedit/TextPainter.h \
    qsynedit/Types.h \
    qsynedit/exporter/synexporter.h \
    qsynedit/exporter/synhtmlexporter.h \
    qsynedit/exporter/synrtfexporter.h \
    qsynedit/highlighter/asm.h \
    qsynedit/highlighter/base.h \
    qsynedit/highlighter/composition.h \
    qsynedit/highlighter/cpp.h \
    settingsdialog/compilersetdirectorieswidget.h \
    settingsdialog/compilersetoptionwidget.h \
    settings.h \
    settingsdialog/editorclipboardwidget.h \
    settingsdialog/editorcolorschemewidget.h \
    settingsdialog/editorfontwidget.h \
    settingsdialog/editorgeneralwidget.h \
    settingsdialog/editorsymbolcompletionwidget.h \
    settingsdialog/editorsyntaxcheckwidget.h \
    settingsdialog/environmentappearencewidget.h \
    settingsdialog/executorgeneralwidget.h \
    settingsdialog/settingsdialog.h \
    settingsdialog/settingswidget.h \
    systemconsts.h \
    utils.h \
    common.h \
    widgets/coloredit.h \
    widgets/consolewidget.h \
    widgets/issuestable.h \
    widgets/qconsole.h \
    widgets/qpatchedcombobox.h

FORMS += \
    settingsdialog/debuggeneralwidget.ui \
    widgets/cpudialog.ui \
    mainwindow.ui \
    settingsdialog/compilersetdirectorieswidget.ui \
    settingsdialog/compilersetoptionwidget.ui \
    settingsdialog/editorclipboardwidget.ui \
    settingsdialog/editorcolorschemewidget.ui \
    settingsdialog/editorfontwidget.ui \
    settingsdialog/editorgeneralwidget.ui \
    settingsdialog/editorsymbolcompletionwidget.ui \
    settingsdialog/editorsyntaxcheckwidget.ui \
    settingsdialog/environmentappearencewidget.ui \
    settingsdialog/executorgeneralwidget.ui \
    settingsdialog/settingsdialog.ui

TRANSLATIONS += \
    RedPandaIDE_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    colorschemes.qrc \
    themes/dark/dark.qrc \
    themes/light/light.qrc \
    themes/dracula/dracula.qrc \
    icons.qrc \
    translations.qrc

#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../QScintilla/src/release/ -lqscintilla2_qt5d
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../QScintilla/src/debug/ -lqscintilla2_qt5d
#else:unix: LIBS += -L$$OUT_PWD/../../QScintilla/src/ -lqscintilla2_qt5d

#INCLUDEPATH += $$PWD/../../QScintilla/src
#DEPENDPATH += $$PWD/../../QScintilla/src

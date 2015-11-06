
TARGET = LangConverter
QT +=  widgets core

HEADERS += \
    mainwindow.h \
    finddialog.h \
    androidparser.h \
    language.h \
    parser.h \
    platformdialog.h \
    windowsparser.h \
    macosxparser.h
SOURCES += main.cpp \
    mainwindow.cpp \
    finddialog.cpp \
    androidparser.cpp \
    language.cpp \
    parser.cpp \
    platformdialog.cpp \
    windowsparser.cpp \
    macosxparser.cpp
RESOURCES += grades.qrc

#Set QtXlsxWriter library
include(.\xlsx\qtxlsx.pri)

# install
#target.path = $$[QT_INSTALL_EXAMPLES]/widgets/itemviews/frozencolumn
INSTALLS += target


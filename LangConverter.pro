
TARGET = LangConverter
QT +=  widgets core

CONFIG += c++11 app_bundle

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
QXLSX_PARENTPATH=./         # current QXlsx path is . (. means curret directory)
QXLSX_HEADERPATH=./header/  # current QXlsx header path is ./header/
QXLSX_SOURCEPATH=./source/  # current QXlsx source path is ./source/
include(./QXlsx.pri)

# install
#target.path = $$[QT_INSTALL_EXAMPLES]/widgets/itemviews/frozencolumn
INSTALLS += target


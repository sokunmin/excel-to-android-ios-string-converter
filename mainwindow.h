#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QMap>
#include "platformdialog.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QTableWidget;
class QTableWidgetItem;
class FindDialog;
class Parser;
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:

    void setupContextMenu();

private slots:
    void open();
    void save();
    void find();
    void updateColor(QTableWidgetItem *item);
    void selectColor();
    void findNext(const QString &text, bool checkWholeWord);
    void findPrev(const QString &text, bool checkWholeWord);
    void platformSelect(const int index);
private:
    bool isOpenMode;
    Parser *parser;
    int curSearchIndex;
    QTableWidgetItem *prevCell;
    QTableWidget *tableView;

    //function menu / actions
    QMenu *fileMenu;
    QAction *colorAct;
    QAction *importAct;
    QAction *exportAct;

    QMenu *editMenu;
    QAction *findAct;

    FindDialog *findDialog;
    PlatformDialog *platformDialog;
    int selectedPlatform;

    QDir currentDir;
    void createItem(int row, int col, const QString &text, const QColor &textColor, const QColor &bgColor, bool isEditable = true, int spanRowCount = 0, int spanColCount = 0);
    int findIndexOfColumn(const QString &header);
    void findCell(const QString &text);
    void openExcel(const QString &input);
    void saveToExcel(const QString &output, const QTableWidget *table);

};

#endif // MAINWINDOW_H

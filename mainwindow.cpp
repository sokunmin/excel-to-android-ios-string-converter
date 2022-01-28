

#include <QtWidgets>
#include <QStandardItemModel>
#include <QFile>
#include <QDebug>
#include "mainwindow.h"
#include "finddialog.h"
#include "language.h"
#include "xlsxdocument.h"
#include "xlsxformat.h"
#include "xlsxcellformula.h"
#include "xlsxcellrange.h"
#include "androidparser.h"
#include "windowsparser.h"
#include "macosxparser.h"

/*
https://sourcemaking.com/design_patterns/template_method/cpp/2
https://sourcemaking.com/design_patterns/template_method/cpp/1
https://sourcemaking.com/design_patterns/factory_method/cpp/1
http://en.wikibooks.org/wiki/C%2B%2B_Programming/Code/Design_Patterns
http://pydoing.blogspot.tw/2012/10/cpp-class.html
http://openhome.cc/Gossip/CppGossip/ClassABC.html
http://www.cnblogs.com/oomusou/archive/2007/03/31/695076.html
*/
using namespace QXlsx;

MainWindow::MainWindow() : curSearchIndex(0),
    prevCell(0), isOpenMode(true)
{
    currentDir = QDir::current();
    //create menu &
    importAct = new QAction(tr("&Import File(s)..."), this);
    importAct->setShortcuts(QKeySequence::Open);
    importAct->setStatusTip(tr("Open files"));
    connect(importAct, SIGNAL(triggered()), this, SLOT(open()));

    exportAct = new QAction(tr("&Export to..."), this);
    exportAct->setShortcuts(QKeySequence::Save);
    exportAct->setStatusTip(tr("Save files"));
    connect(exportAct, SIGNAL(triggered()), this, SLOT(save()));

    findAct = new QAction(tr("&Find... "), this);
    findAct->setShortcuts(QKeySequence::Find);
    findAct->setStatusTip(tr("Find the word among the cells"));
    connect(findAct, SIGNAL(triggered()), this, SLOT(find()));

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(importAct);
    fileMenu->addAction(exportAct);
    fileMenu->addAction(findAct);
    setupContextMenu();

    tableView = new QTableWidget(0, 1);
    tableView->setWindowTitle(QObject::tr("Multiple Language Converter"));
    tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tableView->setShowGrid(true);
    tableView->setAutoScroll(true);
    tableView->resize(860, 680);
    tableView->show();
    this->resize(860, 680);
    setCentralWidget(tableView);

    connect(tableView, SIGNAL(currentItemChanged(QTableWidgetItem*, QTableWidgetItem*)), this , SLOT(updateColor(QTableWidgetItem*)));

    findDialog = new FindDialog(this);
    platformDialog = new PlatformDialog(this);

    connect(platformDialog, SIGNAL(ok(int)), this, SLOT(platformSelect(int)));
    connect(findDialog, SIGNAL(findNext(QString, bool)), this, SLOT(findNext(QString, bool)));
    connect(findDialog, SIGNAL(findPrev(QString, bool)), this, SLOT(findPrev(QString, bool)));

}


void MainWindow::setupContextMenu()
{
    colorAct = new QAction(QPixmap(16, 16), tr("Background &Color..."), this);
    connect(colorAct, SIGNAL(triggered()), this, SLOT(selectColor()));
    setContextMenuPolicy(Qt::ActionsContextMenu);
    addAction(colorAct);
}

void MainWindow::open()
{
    isOpenMode = true;
    qDebug() << Q_FUNC_INFO;
    platformDialog->show();
}

void MainWindow::save()
{
    isOpenMode = false;
    platformDialog->show();
}

void MainWindow::find()
{
    findDialog->show();
}

void MainWindow::findNext(const QString &text, bool checkWholeWord)
{
    qDebug() << Q_FUNC_INFO;
    Qt::MatchFlags match = (!checkWholeWord) ? Qt::MatchContains : Qt::MatchExactly;
    QList<QTableWidgetItem *> table = tableView->findItems(text, match);

    if (prevCell != NULL)
        prevCell->setSelected(false);

    if (table.count() > 0){
        while(curSearchIndex < table.count()) {
            QTableWidgetItem * item = table.at(curSearchIndex);
            item->setSelected(true);
            tableView->scrollToItem(item);
            prevCell = item;
            ++curSearchIndex;
            break;
        }

        if (curSearchIndex >= table.count())
            curSearchIndex = 0;
    } else {
        QMessageBox::warning(this, "Warning", text + " not found!");
    }
}

void MainWindow::findPrev(const QString &text, bool checkWholeWord)
{
    qDebug() << Q_FUNC_INFO;
    Qt::MatchFlags match = (!checkWholeWord) ? Qt::MatchContains : Qt::MatchExactly;
    QList<QTableWidgetItem *> table = tableView->findItems(text, match);

    if (prevCell != NULL)
        prevCell->setSelected(false);

    if (table.count() > 0){
        while(curSearchIndex >= 0) {
            QTableWidgetItem * item = table.at(curSearchIndex);
            item->setSelected(true);
            tableView->scrollToItem(item);
            prevCell = item;
            --curSearchIndex;
            break;
        }

        if (curSearchIndex < 0)
            curSearchIndex = (table.count()-1);
    } else {
        QMessageBox::warning(this, "Warning", text + " not found!");
    }
}

int MainWindow::findIndexOfColumn(const QString &header)
{
    for (int i=0; i<tableView->columnCount(); ++i) {
        if (tableView->horizontalHeaderItem(i)->text() == header) {
            return i;
        }
    }
    return -1;
}


void MainWindow::platformSelect(const int index)
{
    qDebug() << Q_FUNC_INFO;
    selectedPlatform = index;

    if (isOpenMode) {
        QString fileType;
        QStringList files;

        switch(index) {
        case Platform::ANDROID: {
            fileType = "XML files(*.xml)";
            parser = new AndroidParser(this);
            files = QFileDialog::getOpenFileNames(this, "Select one or more files to open", currentDir.absolutePath(), fileType);
            break;
        }
        case Platform::IOS: {
            fileType = "Strings files(*.strings)";
            parser = new MacOSXParser(this);
            files = QFileDialog::getOpenFileNames(this, "Select one or more files to open", currentDir.absolutePath(), fileType);
            break;
        }
        case Platform::WEB: {
            QMessageBox::warning(this, "WARN", "Web output is not implemented yet!");
            break;
        }
        case Platform::EXCEL: {
            fileType = "Excel 2007(*.xlsx)";
            QString file = QFileDialog::getOpenFileName(this, "Select one file to open", currentDir.absolutePath(), fileType);

            qDebug() << "File selected: " << file;
            if (file != NULL) {
                openExcel(file);
                currentDir = QDir(QFileInfo(file).absoluteFilePath());
                qDebug() << "Now path is ..." << currentDir.absolutePath();
            }

            return;
        }
        default:
            fileType = "XML files(*.xml);;Strings files(*.strings);;Excel 2007(*.xlsx)";
            break;
        }

        //Dialog that is used to select files, and return file list.

        if (!files.isEmpty()) {
            tableView->clear();
            currentDir = QDir(QFileInfo(files.at(0)).absoluteFilePath());
            QStringList headers = parser->parse(currentDir, files);
            if (headers.isEmpty() || headers.size() == 0) {
                return;
            }
            headers.insert(0, "Id");
            tableView->setRowCount(parser->getLanguages().size());
            tableView->setColumnCount(headers.count());
            tableView->setHorizontalHeaderLabels(headers);
            for (int i=0; i<headers.count(); i++) {
                switch (i) {
                case 0:
                    tableView->setColumnWidth(0, 250);
                    break;
                default:
                    tableView->setColumnWidth(i, 260);
                    break;
                }
            }

            int row = 0;
            foreach ( Language *lang, parser->getLanguages()) {
                QMapIterator<QString, QString> i(lang->map);
                //iterator
                if (lang->isComment) {
                    QColor bgColor = lang->colors[headers[1]];
                    createItem(row, 0, lang->name, QColor(Qt::black), bgColor, false, 1, headers.count());
                } else {
                    createItem(row, 0, lang->name, QColor(Qt::black), QColor("#FFFFFF"), false, 0);
                    while (i.hasNext()) {
                        i.next();
                        createItem(row, findIndexOfColumn(i.key()), i.value(), QColor(Qt::black), lang->colors.find(i.key()).value());
                    }
                }
                ++row;
            }
            tableView->resizeRowsToContents();
        }
    } else {

        QString directory = QFileDialog::getExistingDirectory(this, tr("Save to Folder..."), currentDir.absolutePath());
        if (!directory.isEmpty()) {
            currentDir = QDir(directory);
            switch(index) {
            case Platform::ANDROID: {
                parser = new AndroidParser(this);
                parser->save(directory, tableView);
                break;
            }
            case Platform::IOS: {
                parser = new MacOSXParser(this);
                parser->save(directory, tableView);
                break;
            }
            case Platform::WEB: {
                QMessageBox::warning(this, "WARN", "Web output is not implemented yet!");
                break;
            }
            case Platform::EXCEL: {
                saveToExcel(directory, tableView);
                break;
            }
            }
            QMessageBox::information(this, "DONE", "Files exported successfully!");
        }
    }
}

void MainWindow::openExcel(const QString &input)
{
    qDebug() << Q_FUNC_INFO;
    tableView->clear();
    QXlsx::Document xlsx(input);
    int rowCount = 0, colCount = 0;

    colCount = xlsx.dimension().lastColumn() ;
    rowCount = xlsx.dimension().lastRow();

    tableView->setRowCount(rowCount-1);
    tableView->setColumnCount(colCount);
    qDebug() << QString("ColCount:%1, RowCount:%2 ").arg(colCount).arg(rowCount);
    // https://www.twblogs.net/a/5beae0022b717720b51e8bcb
    QList<CellRange> mergedCellRangeList = xlsx.currentWorksheet()->mergedCells();

    for (int row = 0; row < rowCount; ++row) {
        QString mergedCellText = "";
        for (int col = 0; col < colCount; ++col) {
            tableView->setColumnWidth(col, 250);
            if (Cell *cell = xlsx.cellAt(row+1, col+1)) {
                QColor fontColor = cell->format().fontColor();
                QColor bgColor = cell->format().patternBackgroundColor();
                QString cellText = cell->value().toString();
                qDebug() << QString("Current CellText(row:%1, col:%2)=%3, %4").arg(row).arg(col).arg(cellText).arg(bgColor.name());
                if (row == 0) {
                    QTableWidgetItem * item = new QTableWidgetItem(cellText);
                    tableView->setHorizontalHeaderItem(col, item);
                    item->setBackground(bgColor);
                } else {
                    if(mergedCellRangeList.size() > 0) {
                        bool isCellSpan = false;
                        int spanColCount = 0;
                        int spanRowCount = 0;
                        foreach(CellRange cellRange, mergedCellRangeList) {
                            int firstRow = cellRange.firstRow();
                            int lastRow = cellRange.lastRow();
                            int firstCol = cellRange.firstColumn();
                            int lastCol = cellRange.lastColumn();
                            spanColCount = lastCol - firstCol;
                            spanRowCount = lastRow - firstRow;
                            qDebug() << QString("Check SpanCell(r1:%1, r2:%2, c1:%3, c2:%4) / (r:%5, c:%6)")
                                        .arg(firstRow).arg(lastRow).arg(firstCol).arg(lastCol).arg(spanRowCount).arg(spanColCount);
                            if(firstRow <= row + 1 && row + 1 <= lastRow && firstCol <= col + 1 && col + 1 <= lastCol) {
                                QString mergedText = xlsx.read(firstRow, firstCol).value<QString>();
                                mergedCellText += mergedText;
                                isCellSpan = true;
                            }
                        }
                        if(isCellSpan) {
                            qDebug() << QString("Found MergedCell at (%1, %2) for (%3, %4) cells").arg(row).arg(col).arg(spanRowCount).arg(spanColCount);
                            createItem(row - 1, col, mergedCellText, fontColor, bgColor, false, spanRowCount + 1, spanColCount + 1);
                            row += spanRowCount;
                            col += spanColCount;
                            continue;
                        }
                    }
                    if (!cellText.isEmpty())
                        createItem(row - 1, col, cellText, QColor(Qt::black), bgColor, true, 0, 0);
                }
            }
        }
        qDebug() << "\n";
    }

    tableView->resizeRowsToContents();
}



void MainWindow::saveToExcel(const QString &output, const QTableWidget *table)
{
    qDebug() << Q_FUNC_INFO;
    if (!output.isEmpty()) {
        Document xlsx;
        QString saveName = "output";
        int headerCount = table->columnCount();

        Format headerFormat;
        headerFormat.setPatternBackgroundColor(QColor(Qt::black));
        headerFormat.setHorizontalAlignment(Format::AlignHCenter);
        headerFormat.setVerticalAlignment(Format::AlignVCenter);
        headerFormat.setFontColor(QColor(Qt::white));

        Format footFormat;
        footFormat.setHorizontalAlignment(Format::AlignHCenter);
        footFormat.setVerticalAlignment(Format::AlignVCenter);
        footFormat.setPatternBackgroundColor(QColor(Qt::red));
        footFormat.setFontColor(QColor(Qt::white));

        xlsx.write(1, 1, "Id", headerFormat);
        xlsx.setColumnWidth(1, 45); //Id column width

        for (int col = 1; col < headerCount; ++col) {
            QString filename = table->horizontalHeaderItem(col)->text();
            QFileInfo fileInfo(filename);

            xlsx.write(1, col+1, saveName, headerFormat);

            for (int row = 0; row < table->rowCount(); ++row) {
                Format format;
                format.setHorizontalAlignment(Format::AlignLeft);
                format.setVerticalAlignment(Format::AlignVCenter);
                format.setFontSize(12);
                format.setFontColor(QColor(Qt::black));
//                format.setFontName("微軟正黑體");
                format.setFontName("Arial");
                format.setTextWrap(true);
                format.setBorderStyle(QXlsx::Format::BorderThin);

                //index begins from 1 instead of 0.
                if (table->columnSpan(row, 0) == headerCount) { //[header] spanned row
                    QTableWidgetItem * cell = table->item(row, 0);
                    if (cell != NULL) {
                        QColor color = cell->backgroundColor();
                        format.setPatternBackgroundColor(color);
                        xlsx.write(row+2, 1, cell->text(), format);
                        xlsx.mergeCells(CellRange(row+2, 1, row+2, headerCount), format); //Span to all columns
                    }
                } else {
                    QTableWidgetItem * cell = table->item(row, col);
                    if (cell != NULL) {
                        QColor color = cell->backgroundColor();
                        format.setPatternBackgroundColor(color);
                        xlsx.write(row+2, col+1, cell->text(), format); //Content column
                        xlsx.write(row+2, 1, table->item(row, 0)->text(), format); //Id column
                        xlsx.setColumnWidth(col+1, 60); //Content column width
                    }
                }
            }
        }

        saveName = output + "/" + saveName + ".xlsx";
        xlsx.saveAs(saveName);
    }
}


void MainWindow::createItem(int row, int col, const QString &text,
                            const QColor &textColor, const QColor &bgColor,
                            bool isEditable, int spanRowCount, int spanColCount)
{
    QTableWidgetItem *item = new QTableWidgetItem(text);
    if (!isEditable) {
        item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        QFont font;
        font.setBold(true);
        font.setPointSize(10);
        item->setFont(font);
    }
    item->setTextAlignment(Qt::AlignVCenter);
    item->setBackgroundColor(bgColor);
    tableView->setItem(row, col, item);
    if (spanColCount > 0) {
        tableView->setSpan(row, col, spanRowCount, spanColCount);
        tableView->item(row, col)->setTextColor(textColor);
    }
}


void MainWindow::selectColor()
{
    QTableWidgetItem *item = tableView->currentItem();
    QColor col = item ? item->backgroundColor() : tableView->palette().base().color();
    col = QColorDialog::getColor(col, this);
    if (!col.isValid())
        return;

    QList<QTableWidgetItem*> selected = tableView->selectedItems();
    if (selected.count() == 0)
        return;

    foreach (QTableWidgetItem *i, selected) {
        if (i)
            i->setBackgroundColor(col);
    }

    updateColor(tableView->currentItem());
}

void MainWindow::updateColor(QTableWidgetItem *item)
{
    QPixmap pix(16, 16);
    QColor col;
    if (item)
        col = item->backgroundColor();
    if (!col.isValid())
        col = palette().base().color();

    QPainter pt(&pix);
    pt.fillRect(0, 0, 16, 16, col);

    QColor lighter = col.light();
    pt.setPen(lighter);
    QPoint lightFrame[] = { QPoint(0, 15), QPoint(0, 0), QPoint(15, 0) };
    pt.drawPolyline(lightFrame, 3);

    pt.setPen(col.dark());
    QPoint darkFrame[] = { QPoint(1, 15), QPoint(15, 15), QPoint(15, 1) };
    pt.drawPolyline(darkFrame, 3);

    pt.end();

    colorAct->setIcon(pix);
}

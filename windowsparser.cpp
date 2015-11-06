#include "windowsparser.h"
#include "language.h"
#include <QDebug>
#include <QTableWidget>
#include <QSettings>

const QString XML_CONTENT_1 = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<resources>\n";
const QString XML_CONTENT_2 = "<translation revision=\"0.9\" language=\"%1\" languageInEnglish=\"%2\" author=\"XYZ\">";
const QString XML_CONTENT_3 = "</translation>";
const QString XML_CONTENT_4 = "</resource>";

WindowsParser::WindowsParser(QObject *parent):Parser(parent)
{
    //read windows.ini
    QSettings settings("windows_languages.ini", QSettings::IniFormat);
    foreach (const QString &group, settings.childGroups()) {
        settings.beginGroup(group);
        Ini ini;
        ini.languageName = settings.value("language").toString();
        ini.languageInEnglish = settings.value("languageInEnglish").toString();

        translation.insert(group, ini);
        settings.endGroup();
    }
}

QStringList WindowsParser::parse(const QDir &dir, const QStringList &files)
{
    QString pattern1 = "<t\\s+id=\"([^\"]+[^\">])\">(.*)<\\/t>(.*)";
    QString pattern2 = "\\s*<!--\\s*(.*)\\s*-->";

    QRegExp rx(pattern1);
    Qt::CaseSensitivity cs = Qt::CaseInsensitive;

    rx.setCaseSensitivity(cs);

    QStringList headers;

    for (int i = 0; i < files.size(); ++i) {
        QFile file(dir.absoluteFilePath(files[i]));

        if (file.open(QIODevice::ReadOnly)) {
            QFileInfo fileInfo(file.fileName());
            headers << fileInfo.baseName();

            QString line;
            QTextStream in(&file);
            in.setCodec("UTF-8");
            int row = 0;
            while (!in.atEnd()) {
                line = in.readLine();

                QRegExp commentx(pattern2);
                if (commentx.exactMatch(line)) { //Is it a comment?
                    commentx.indexIn(line);
                    if (!isNameExisted(commentx.cap(1).trimmed())) { //Is the comment added in the map?
                        Language * lang = new Language(this);
                        lang->add(commentx.cap(1).trimmed(), fileInfo.baseName(), "", QColor("#EEEEEE"));
                        lang->isComment = true;
                        langMap.append(lang);
                    }
                } else {
                    int pos = 0;

                    //parse content
                    while ( (pos = rx.indexIn(line, pos)) != -1) {
                        for (int k = 1; k <= rx.captureCount(); ++k) {
                            QRegExp colorx(pattern2);
                            colorx.indexIn(rx.cap(3));
                            if (colorx.captureCount() > 0 && colorx.cap(1).trimmed().length() > 0) {
                                add(rx.cap(1), fileInfo.baseName(), rx.cap(2), QColor(colorx.cap(1).trimmed()));
                            } else {
                                add(rx.cap(1), fileInfo.baseName(), rx.cap(2), QColor("#FFFFFF"));
                            }

                            //qDebug() << fileInfo.baseName() << " = " << rx.cap(2);
                        }
                        pos += rx.matchedLength();

                        ++row;
                    }
                }
            }
        }
    }

    return headers;
}


void WindowsParser::save(const QString &output, const QTableWidget *table)
{
    if (!output.isEmpty()) {


        //save content
        for (int i = 1; i < table->columnCount(); ++i) {
            QString language = table->horizontalHeaderItem(i)->text();
            QString filename = language + ".xml";
            QFile file(output + "/" + (filename));

            if (file.open(QIODevice::WriteOnly)) {
                file.write("\xEF\xBB\xBF"); //Write UTF-8 with BOM
                file.setTextModeEnabled(true); // Enable text mode
                QTextStream output(&file);
                output.setCodec("UTF-8");
                output.setGenerateByteOrderMark(true);

                Ini ini = translation[language];

                QString content = XML_CONTENT_1 + "\n";
                content.append(QString(XML_CONTENT_2).arg(ini.languageName).arg(ini.languageInEnglish)).append("\n") ;
                for (int k = 0; k < table->rowCount(); ++k) {
                    if (table->columnSpan(k, 0) == table->columnCount()) { //span row / header
                        content.append("\n	<!-- ").append(table->item(k, 0)->text()).append(" -->\n");
                    } else {

                        QString color = table->item(k, i)->backgroundColor().name();
                        color = (color.compare("#FFFFFF", Qt::CaseInsensitive))?color:"";
                        if (color != "")
                            color = "<!-- " + color + " -->";

                        content.append("<t id=\"")
                               .append(table->item(k, 0)->text())
                               .append("\">")
                               .append(table->item(k, i)->text())
                               .append("</t>")
                               .append(color)
                               .append("\n");
                    }
                }
                content.append(XML_CONTENT_3);
                content.append(XML_CONTENT_4);
                output << content.toUtf8();
            }
            file.close();
        }
    }
}

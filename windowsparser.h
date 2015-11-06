#ifndef WINDOWSPARSER_H
#define WINDOWSPARSER_H

#include "parser.h"
#include <QDir>
#include <QMap>

class WindowsParser : public Parser
{
public:
    typedef struct {
        QString languageName;
        QString languageInEnglish;
    }Ini;


    WindowsParser(QObject *parent = 0);
    virtual QStringList parse(const QDir &dir, const QStringList &files);
    virtual void save(const QString &output, const QTableWidget *table);

private:
    QMap<QString, Ini> translation;
};

#endif // WINDOWSPARSER_H

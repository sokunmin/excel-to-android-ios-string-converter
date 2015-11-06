#ifndef ANDROIDPARSER_H
#define ANDROIDPARSER_H

#include "parser.h"
#include <QDir>

class AndroidParser : public Parser
{
public:
    AndroidParser(QObject *parent = 0);


    virtual QStringList parse(const QDir &dir, const QStringList &files);
    virtual void save(const QString &output, const QTableWidget *table);
};

#endif // ANDROIDPARSER_H

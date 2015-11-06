#ifndef MACOSXPARSER_H
#define MACOSXPARSER_H

#include "parser.h"
#include <QDir>

class MacOSXParser : public Parser
{
public:
    MacOSXParser(QObject *parent = 0);

    virtual QStringList parse(const QDir &dir, const QStringList &files);
    virtual void save(const QString &output, const QTableWidget *table);
};

#endif // MACOSXPARSER_H

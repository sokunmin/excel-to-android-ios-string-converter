#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QDir>


QT_BEGIN_NAMESPACE
class Language;
class QTableWidget;
QT_END_NAMESPACE

class Parser : public QObject
{
    Q_OBJECT
public:
    explicit Parser(QObject *parent = 0);
    virtual QStringList parse(const QDir &dir, const QStringList &files) = 0;
    virtual void save(const QString &output, const QTableWidget *table) = 0;
    bool isNameExisted(const QString &name);
    QList<Language *> getLanguages();

    void add(const QString &name, const QString &country, const QString &content, const QColor &color);
protected:

    QList<Language *> langMap;

private:

};

#endif // PARSER_H

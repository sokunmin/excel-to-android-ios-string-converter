#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <QObject>
#include <QMap>
#include <QColor>

class Language : QObject
{
    Q_OBJECT
public:
    Language(QObject *parent = 0);
    bool isComment;
    QString name;
    QColor color;
    QMap<QString, QString> map;
    QMap<QString, QColor> colors;
    void add(const QString &name, const QString &country, const QString &lang, const QColor &color);

};

#endif // LANGUAGE_H

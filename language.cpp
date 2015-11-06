#include "language.h"

Language::Language(QObject *parent):QObject(parent)
{
}



void Language::add(const QString &name, const QString &country, const QString &lang, const QColor &color) {
    this->name = name;
    map.insert(country, lang);
    colors.insert(country, color);
};

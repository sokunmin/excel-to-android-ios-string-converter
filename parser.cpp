#include "parser.h"
#include "language.h"
#include "xlsxdocument.h"
#include "xlsxformat.h"
#include <QDebug>
#include <QTableWidget>
#include <QTableWidgetItem>

QTXLSX_USE_NAMESPACE

const QString LETTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const QString ENDTAG = "<!-- ENDTAG -->";

Parser::Parser(QObject *parent):QObject(parent)
{
}


bool Parser::isNameExisted(const QString &name)
{
    foreach (Language *lang, langMap) {
        if (lang->name.toLower() == name.toLower()) {
            return true;
        }
    }
    return false;
}

QList<Language *> Parser::getLanguages()
{
    return langMap;
}


void Parser::add(const QString &name, const QString &country, const QString &content, const QColor &color)
{
    bool isNameFound = false;
    foreach ( Language *lang, langMap) {
        if (lang->name == name) {
            isNameFound = true;
            lang->map.insert(country, content);
            lang->colors.insert(country, color);
            return;
        }
    }

    if(!isNameFound) {
        Language *newLang = new Language();
        newLang->add(name, country, content, color);
        newLang->isComment = false;
        langMap << newLang;
    }
}

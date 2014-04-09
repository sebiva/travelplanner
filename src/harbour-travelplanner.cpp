/*
    Copyright (C) 2014 Sebastian Ivarsson
    Contact: Sebastian Ivarsson <sebiva@student.chalmers.se>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <QTranslator>
#include <QTextCodec>
#include <QLocale>

#include <QtQml/QQmlEngine>
#include <qqml.h>
#include "src/parser.h"
#include "src/search.h"
#include <sailfishapp.h>
#include "timehelper.h"


int main(int argc, char *argv[])
{
    Parser::getinstance();
    qmlRegisterType<Search>("searcher", 1, 0, "Search");
    qmlRegisterType<Timehelper>("timehelp", 1,0, "Timehelp");
    //qmlRegisterSingletonType<Parser>("cppParser", 1, 0,"Parser", Parser::qobject_singletontype_provider);

    QGuiApplication * app = SailfishApp::application(argc, argv);


    //Translations



    QString langCode(getenv("LANG"));
    qDebug() << "Code" << langCode;
    if (langCode.isEmpty() || langCode == "C" || !langCode.contains("_"))
        langCode = QLocale::system().name();
    if (langCode.contains('.'))
        langCode = langCode.mid(0, langCode.lastIndexOf('.'));
    QString filename = QString("languages/harbour_travelplanner_") + langCode;

    qDebug() << "Language Code:" << langCode;
    static QTranslator translator;
    if( translator.load(filename, ":/") ){
        app->installTranslator(&translator);
        //QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
        qDebug() << "Translation file loaded" << filename;
    } else
        qDebug() << "Translation file not loaded:" << filename;


    return SailfishApp::main(argc, argv);
}


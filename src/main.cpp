/*
    This file is part of Qstit - http://subtitles.nova-cinema.org
    Feedback, comments and questions are welcome: subtitles@nova-cinema.org
   ════════════════════════════════════════════════════════════════════════════════════
    Qstit is developed by Nova Cinema, Brussels - http://www.nova-cinema.org
    Conception: Laurent Tenzer
    Programming: Georges Piedboeuf-Boen - georges.pi.bo@gmail.com
    Icons credit: Nova & Axialis Team - http://www.axialis.com/free/icons
   ════════════════════════════════════════════════════════════════════════════════════
    Qstit is a free software: you can redistribute it and/or modify it under the terms
    of the GNU General Public License as published by the Free Software Foundation,
    either version 3 of the License, or (at your option) any later version.
    If you do, we'd like to hear about it.

    Qstit is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
    without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
    PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with Qstit.
    If not, see http://www.gnu.org/licenses
   ════════════════════════════════════════════════════════════════════════════════════
*/

#include <QApplication>
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    #include <QCleanlooksStyle>
    #include <QTextCodec>
#else
    #include <QStyleFactory>
#endif
#include "QSTit_winMain.h"

int main(int argc,char *argv[])
{
    QApplication app(argc, argv);
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    // Cleanlooks was dropped in Qt5; "Fusion" is its successor.
    QApplication::setStyle(new QCleanlooksStyle);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
#else
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    // Qt5+ already treats C string literals as UTF-8.
#endif

    winMain wMain;
    wMain.show();
    return app.exec();
}

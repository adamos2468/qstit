/*
    This file is part of Qstit - http://subtitles.nova-cinema.org
    Feedback, comments and questions are welcome: subtitles@nova-cinema.org
   ════════════════════════════════════════════════════════════════════════════════════
    Compatibility helpers for the Qt classes that were removed after Qt4.
    Qstit was written against Qt4; these wrappers keep the original semantics
    while building against Qt5 and Qt6.
   ════════════════════════════════════════════════════════════════════════════════════
*/

#ifndef QSTIT_COMPAT_H
#define QSTIT_COMPAT_H

#include <QtGlobal>
#include <QRect>
#include <QTextStream>
#include <QFont>
#include <QFontMetrics>
#include <QString>

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    #include <QApplication>
    #include <QDesktopWidget>
#else
    #include <QGuiApplication>
    #include <QScreen>
#endif

namespace qstit
{
    // Qt4's QDesktopWidget reported the bounding box of every screen ("virtual desktop"),
    // which QDesktopWidget itself was dropped for in Qt6. QScreen exposes the same rect.
    inline QRect deskGeom()
    {
    #if QT_VERSION < QT_VERSION_CHECK(5,0,0)
        return QApplication::desktop()->geometry();
    #else
        const QScreen *scr=QGuiApplication::primaryScreen();
        return scr ? scr->virtualGeometry() : QRect();
    #endif
    }

    // QTextStream::setCodec() went away with QTextCodec in Qt6.
    inline void strmEncoLati(QTextStream &strm)
    {
    #if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        strm.setCodec("ISO-8859-1");
    #else
        strm.setEncoding(QStringConverter::Latin1);
    #endif
    }
    inline void strmEncoUtf8(QTextStream &strm)
    {
    #if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        strm.setCodec("UTF-8");
    #else
        strm.setEncoding(QStringConverter::Utf8);
    #endif
    }

    // Qt4/Qt5 weighted fonts on a 0-99 scale (Normal=50, Bold=75); Qt6 uses the
    // OpenType/CSS scale (Normal=400, Bold=700). Qstit stores the weight in its
    // config file, so both scales have to be accepted on the way in - old config
    // files hold legacy values, new ones hold OpenType values.
    // The pairs below are the mapping Qt itself uses between the two scales.
    inline int weigLegaToOpen(int legaWeig)
    {
        static const int aLega[]={0,12,25,50,57,63,75,81,87};
        static const int aOpen[]={100,200,300,400,500,600,700,800,900};
        const int iN=9;
        if (legaWeig<=aLega[0]) return aOpen[0];
        for (int i=1;i<iN;i++) {
            if (legaWeig>aLega[i]) continue;
            const int iSpan=aLega[i]-aLega[i-1];
            return aOpen[i-1]+((legaWeig-aLega[i-1])*(aOpen[i]-aOpen[i-1])+iSpan/2)/iSpan;
        }
        return aOpen[iN-1];
    }
    inline int weigOpenToLega(int openWeig)
    {
        static const int aLega[]={0,12,25,50,57,63,75,81,87};
        static const int aOpen[]={100,200,300,400,500,600,700,800,900};
        const int iN=9;
        if (openWeig<=aOpen[0]) return aLega[0];
        for (int i=1;i<iN;i++) {
            if (openWeig>aOpen[i]) continue;
            const int iSpan=aOpen[i]-aOpen[i-1];
            return aLega[i-1]+((openWeig-aOpen[i-1])*(aLega[i]-aLega[i-1])+iSpan/2)/iSpan;
        }
        return aLega[iN-1];
    }
    // Sets a weight coming either from the config file or from a hard-coded constant.
    inline void fontWeigSet(QFont &font,int weig)
    {
    #if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        font.setWeight(weig>99 ? weigOpenToLega(weig) : weig);
    #else
        font.setWeight(static_cast<QFont::Weight>(weig<100 ? weigLegaToOpen(weig) : weig));
    #endif
    }

    // QFontMetrics::width() was deprecated in Qt 5.11 and removed in Qt6.
    inline int textWidt(const QFontMetrics &metr,const QString &text)
    {
    #if QT_VERSION < QT_VERSION_CHECK(5,11,0)
        return metr.width(text);
    #else
        return metr.horizontalAdvance(text);
    #endif
    }

    // Like Qt4's QDesktopWidget::availableGeometry() with no argument: the primary
    // screen minus the space reserved by panels / taskbars.
    inline QRect deskAvai()
    {
    #if QT_VERSION < QT_VERSION_CHECK(5,0,0)
        return QApplication::desktop()->availableGeometry();
    #else
        const QScreen *scr=QGuiApplication::primaryScreen();
        return scr ? scr->availableGeometry() : QRect();
    #endif
    }
}

#endif

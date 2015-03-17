/****************************************************************************
**
** Copyright (C) 2014 - 2015 Deif Lou
**
** This file is part of Anitools
**
** Anitools is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#ifndef ANITOOLS_MISC_NATIVEEVENTFILTER_H
#define ANITOOLS_MISC_NATIVEEVENTFILTER_H

#include <QObject>
#include <QAbstractNativeEventFilter>
#include <QByteArray>
#include <QHash>
#include <QApplication>

namespace anitools {
namespace misc {

class NativeEventFilter : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT

public:
    NativeEventFilter(QObject *parent = 0) :
        QObject(parent)
    {
        qApp->installNativeEventFilter(this);
    }

    bool nativeEventFilter(const QByteArray & eventType, void * message, long * result);
    inline void registerMessage(unsigned int msg, bool takeOwnership = false)
    {
        mRegisteredMessages.insert(msg, takeOwnership);
    }

    inline void unregisterMessage(unsigned int msg)
    {
        mRegisteredMessages.remove(msg);
    }

private:
    QHash<unsigned int, bool> mRegisteredMessages;

signals:
    void nativeEvent(void * message, long * result);
};

}}

#endif // ANITOOLS_MISC_NATIVEEVENTFILTER_H

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

#include "nativeeventfilter.h"

#ifdef Q_OS_WIN32
#include <windows.h>
#endif

namespace anitools {
namespace misc {

bool NativeEventFilter::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
#ifdef Q_OS_WIN32
    if (eventType != "windows_generic_MSG")
        return false;

    MSG * msg = (MSG *)message;
    unsigned int lw = LOWORD(msg->message);
    if (mRegisteredMessages.contains(lw))
    {
        emit nativeEvent(message, result);
        return mRegisteredMessages.value(lw);
    }
#endif

    return false;
}

}}

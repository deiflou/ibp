//
// MIT License
// 
// Copyright (c) Deif Lou
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#ifndef IBP_MISC_NATIVEEVENTFILTER_H
#define IBP_MISC_NATIVEEVENTFILTER_H

#include <QObject>
#include <QAbstractNativeEventFilter>
#include <QByteArray>
#include <QHash>
#include <QApplication>

namespace ibp {
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

#endif // IBP_MISC_NATIVEEVENTFILTER_H

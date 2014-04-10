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

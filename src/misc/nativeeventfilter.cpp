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

#ifndef ANITOOLS_IMGPROC_UTIL_H
#define ANITOOLS_IMGPROC_UTIL_H

#include <QStringList>
#include <QString>

#include "types.h"

namespace anitools {
namespace imgproc {

extern QStringList blendModeStrings;
inline BlendMode blendModeStringToEnum(const QString & mode)
{
    if (!blendModeStrings.contains(mode))
        return BlendMode_Unsupported;
    return (BlendMode)blendModeStrings.indexOf(mode.toLower());
}
inline QString blendModeEnumToString(BlendMode mode)
{
    if (mode < BlendMode_Normal || mode > BlendMode_Unsupported)
        return QString();
    return blendModeStrings.at(mode);
}

}}

#endif // ANITOOLS_IMGPROC_UTIL_H

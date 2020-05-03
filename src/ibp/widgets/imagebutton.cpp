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

#include <QMessageBox>

#include "imagebutton.h"
#include "filedialog.h"
#include "../imgproc/freeimage.h"

namespace ibp {
namespace widgets {

using namespace ibp::imgproc;

ImageButton::ImageButton(QWidget *parent) :
    ToolButtonEx(parent)
{
    setExtraDataType(Image);
    setExtraDataFlags(CheckerboardBackground);

    connect(this, SIGNAL(clicked()), this, SLOT(On_Clicked()));
}

void ImageButton::setImage(const QImage & i)
{
    if (i == image())
        return;
    ToolButtonEx::setImage(i);
    emit imageChanged(i);
}

void ImageButton::On_Clicked()
{
    QString fileName;
    fileName = getOpenFileName(this, "images", freeimageGetOpenFilterString());
    if (fileName.isEmpty())
        return;

    QImage img = freeimageLoadAs32Bits(fileName, true);
    if (img.isNull())
    {
        QMessageBox::information(this, QString(), tr("The selected file has an unsupported format."));
        return;
    }

    setImage(img);
}

}}

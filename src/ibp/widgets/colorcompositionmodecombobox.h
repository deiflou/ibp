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

#ifndef IBP_WIDGETS_BLENDMODECOMBOBOX_H
#define IBP_WIDGETS_BLENDMODECOMBOBOX_H

#include <QComboBox>

#include "../misc/util.h"
#include "../imgproc/types.h"

namespace ibp {
namespace widgets {

using namespace ibp::misc;
using namespace ibp::imgproc;

class ColorCompositionModeComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit ColorCompositionModeComboBox(QWidget *parent = 0);
    ColorCompositionMode colorCompositionMode() const;

private:
    inline void addItem(const QString & text, const QVariant & userData = QVariant())
    {
        QComboBox::addItem(text, userData);
    }
    inline void addItem(const QIcon & icon, const QString & text, const QVariant & userData = QVariant())
    {
        QComboBox::addItem(icon, text, userData);
    }
    inline void addItems(const QStringList & texts)
    {
        QComboBox::addItems(texts);
    }
    inline void	insertItem(int index, const QString & text, const QVariant & userData = QVariant())
    {
        QComboBox::insertItem(index, text, userData);
    }
    inline void	insertItem(int index, const QIcon & icon, const QString & text, const QVariant & userData = QVariant())
    {
        QComboBox::insertItem(index, icon, text, userData);
    }
    inline void	insertItems(int index, const QStringList & list)
    {
        QComboBox::insertItems(index, list);
    }
    inline void	insertSeparator(int index)
    {
        QComboBox::insertSeparator(index);
    }
    inline void setCompleter(QCompleter * completer)
    {
        QComboBox::setCompleter(completer);
    }
    inline void setDuplicatesEnabled(bool enable)
    {
        QComboBox::setDuplicatesEnabled(enable);
    }
    inline void setEditable(bool editable)
    {
        QComboBox::setEditable(editable);
    }
    inline void setFrame(bool f)
    {
        QComboBox::setFrame(f);
    }
    inline void setIconSize(const QSize & size)
    {
        QComboBox::setIconSize(size);
    }
    inline void setInsertPolicy(InsertPolicy policy)
    {
        QComboBox::setInsertPolicy(policy);
    }
    inline void setItemData(int index, const QVariant & value, int role = Qt::UserRole)
    {
        QComboBox::setItemData(index, value, role);
    }
    inline void setItemDelegate(QAbstractItemDelegate * delegate)
    {
        QComboBox::setItemDelegate(delegate);
    }
    inline void setItemIcon(int index, const QIcon & icon)
    {
        QComboBox::setItemIcon(index, icon);
    }
    inline void setItemText(int index, const QString & text)
    {
        QComboBox::setItemText(index, text);
    }
    inline void setLineEdit(QLineEdit * edit)
    {
        QComboBox::setLineEdit(edit);
    }
    inline void setMaxCount(int max)
    {
        QComboBox::setMaxCount(max);
    }
    inline void setModel(QAbstractItemModel * model)
    {
        QComboBox::setModel(model);
    }
    inline void setModelColumn(int visibleColumn)
    {
        QComboBox::setModelColumn(visibleColumn);
    }
    inline void setRootModelIndex(const QModelIndex & index)
    {
        QComboBox::setRootModelIndex(index);
    }
    inline void setValidator(const QValidator * validator)
    {
        QComboBox::setValidator(validator);
    }
    inline void setView(QAbstractItemView * itemView)
    {
        QComboBox::setView(itemView);
    }

signals:
    void colorCompositionModeChanged(ColorCompositionMode v);

public slots:
    void setColorCompositionMode(ColorCompositionMode v);

private slots:
    void On_currentIndexChanged(int i);
};

}}

#endif // IBP_WIDGETS_BLENDMODECOMBOBOX_H

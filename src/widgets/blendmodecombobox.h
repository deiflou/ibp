/****************************************************************************
**
** Copyright (C) 2014 Deif Lou
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

#ifndef ANITOOLS_WIDGETS_BLENDMODECOMBOBOX_H
#define ANITOOLS_WIDGETS_BLENDMODECOMBOBOX_H

#include <QComboBox>

#include "../misc/util.h"
#include "../imgproc/types.h"

namespace anitools {
namespace widgets {

using namespace anitools::misc;
using namespace anitools::imgproc;

class BlendModeComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit BlendModeComboBox(QWidget *parent = 0);
    BlendMode blendMode() const;

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
    void blendModeChanged(BlendMode v);

public slots:
    void setBlendMode(BlendMode v);

private slots:
    void On_currentIndexChanged(int i);
};

}}

#endif // ANITOOLS_WIDGETS_BLENDMODECOMBOBOX_H

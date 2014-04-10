#ifndef IMAGEFOLDERLISTPOPUP_H
#define IMAGEFOLDERLISTPOPUP_H

#include <QWidget>
#include <QFileSystemModel>
#include <QTimer>

namespace Ui {
class ImageFolderListPopUp;
}

class ImageFolderListPopUp : public QWidget
{
    Q_OBJECT

public:
    static const int arrowSize = 5;

    explicit ImageFolderListPopUp(QWidget *parent = 0);
    ~ImageFolderListPopUp();

    int countAvailableFiles();
    void setFolder(const QString & folder);
    void setArrowPos(int pos);

protected:
    void paintEvent(QPaintEvent *e);

private:
    Ui::ImageFolderListPopUp *ui;

    QFileSystemModel * mFolderSystemModel;
    QTimer * mTimer;
    QString mFolder;
    bool mIsLoaded;
    QString mTempFolder;
    int mArrowPos;

    static const int alpha = 200;

signals:
    void fileSelected(const QString & fileName);
    void contentsChanged();

private slots:
    void On_mFolderSystemModel_rowsChanged(const QModelIndex & parent, int start, int end);

    void On_mTimer_timeout();
    void on_mListView_activated(const QModelIndex &index);
    void on_mSliderZoom_valueChanged(int value);
};

#endif // IMAGEFOLDERLISTPOPUP_H

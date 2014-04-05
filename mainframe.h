#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <QWidget>
#include <QList>
#include <QRegExp>
#include <QItemSelectionModel>
#include <QClipboard>

#include "videoinfo.h"
#include "videoinfomodel.h"

namespace Ui {
class MainFrame;
}

class MainFrame : public QWidget
{
    Q_OBJECT
    
public:
    explicit MainFrame(QWidget *parent = 0);
    ~MainFrame();

signals:
    void getRealURL();
    void videoInfoChanged(QModelIndex, QModelIndex);

private slots:
    void startParse();
    void videoParsed(int);
    void listAppend();
    void copyAll();
    void copy();
    void about();
    void reset();

private:
    void changeButtonState(bool b);

    Ui::MainFrame *ui;
    QList<VideoInfo*> videolist;
    int count;
    VideoInfoModel model;
    QItemSelectionModel* selection;
    QClipboard* clipboard;
    QString cdata;
};

#endif // MAINFRAME_H

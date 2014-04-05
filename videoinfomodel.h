#ifndef VIDEOINFOMODEL_H
#define VIDEOINFOMODEL_H

#include <QAbstractTableModel>
#include "videoinfo.h"

class VideoInfoModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    VideoInfoModel();
    VideoInfoModel(QList<VideoInfo*>);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());
    void setList(QList<VideoInfo*>);

private:
    QList<VideoInfo*> videolist;
};

#endif // VIDEOINFOMODEL_H

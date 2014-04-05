#include "videoinfomodel.h"
#include "vptr.h"
#include <QTextCodec>

VideoInfoModel::VideoInfoModel()
{
}

VideoInfoModel::VideoInfoModel(QList<VideoInfo *> list)
{
    videolist = list;
}

void VideoInfoModel::setList(QList<VideoInfo *> list)
{
    videolist = list;
}

int VideoInfoModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 4;
}

int VideoInfoModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return videolist.size();
}

QVariant VideoInfoModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return QString::fromUtf8("分段");

            case 1:
                return QString::fromUtf8("编号");

            case 2:
                return QString::fromUtf8("视频名称");

            case 3:
                return QString::fromUtf8("视频地址");

            default:
                return QVariant();
        }
    }
    return QVariant();
}

QVariant VideoInfoModel::data(const QModelIndex &index, int role) const
{
     if (role == Qt::DisplayRole)
     {
         int row = index.row();
         int col = index.column();
         VideoInfo* info = videolist.at(row);

         switch (col)
         {
            case 0:
                if (info->getVideoParts() == 0)
                    return QVariant();
                else
                    return info->getVideoParts();

            case 1:
                return info->getBID();

            case 2:
                return info->getVideoName();

            case 3:
                return info->getRURL();

            default:
                return QVariant();
         }
     }
     return QVariant();
}

bool VideoInfoModel::insertRows(int position, int rows, const QModelIndex &index)
 {
     Q_UNUSED(index);
     beginInsertRows(QModelIndex(), position, position+rows-1);

     for (int row=0; row < rows; row++)
         videolist.append(NULL);

     endInsertRows();
     return true;
 }

bool VideoInfoModel::removeRows(int position, int rows, const QModelIndex &index)
 {
     Q_UNUSED(index);
     beginRemoveRows(QModelIndex(), position, position+rows-1);

     for (int row=0; row < rows; ++row)
         videolist.removeAt(position);

     endRemoveRows();
     return true;
 }

bool VideoInfoModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        int row = index.row(), col = 3;

        VideoInfo* p = VPtr<VideoInfo>::asPtr(value);
        videolist.replace(row, p);

        emit dataChanged(index, index.sibling(row, col));
        return true;
    }
    return false;
}

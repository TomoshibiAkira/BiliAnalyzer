#ifndef VIDEOINFO_H
#define VIDEOINFO_H

#include <QObject>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMimeData>
#include <QBuffer>
#include <QByteArray>

class VideoInfo : public QObject
{
    Q_OBJECT

public:
    VideoInfo();
    VideoInfo(QUrl addr);
    void setVideo(QUrl addr);
    QString getVideoName();
    QString getBID();
    QUrl getBURL();
    QUrl getRURL();
    int getStatus();
    int getVideoParts();
    void setPosition(int p);

signals:
    void parseFinished(int);

public slots:
    void start();

private slots:
    void httpFinished();
    void httpReadyRead();

private:
    bool parseHTML();
    bool parseXML();
    void httpStart(QUrl url);
    void reset();
    void statusReset();

    QString videoRealName, videocid, videobid;
    //      稿件名称         视频cid      视频id
    int status, position, videoParts;
    //   状态    列表位置     分P数
    QUrl videoBAddress, videoRAddress, interface;
    //      B站地址        真实视频地址      接口

    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    bool httpRequestAborted;
    QBuffer buffer;
};

#endif // VIDEOINFO_H

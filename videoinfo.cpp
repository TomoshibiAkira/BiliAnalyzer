#include "videoinfo.h"
#include "status.h"

#include <QDomDocument>
#include <QDomNodeList>
#include <QDomNode>
#include <QStringList>

const QString PLAYER_STR("cid=");
const QString PLAYER_STR2("\"cid\":\"");
const QString INTERFACE_URL("http://interface.bilibili.cn/playurl?cid=");
const QString NAME_STR("<h2 title=\"");

VideoInfo::VideoInfo()
{
    reset();
}

VideoInfo::VideoInfo(QUrl addr)
{
    reset();
    setVideo(addr);
}

void VideoInfo::reset()
{
    statusReset();
    videoBAddress.clear();
    position = -1;
}

void VideoInfo::statusReset()
{
    status = URL_NOT_PARSED;
    videoRealName.clear();
    videocid.clear();
    videoRAddress.clear();
    interface.clear();
    buffer.buffer().clear();
    httpRequestAborted = false;
    videoParts = 0;
}

QString VideoInfo::getBID()
{
    return videobid;
}

QString VideoInfo::getVideoName()
{
    return videoRealName;
}

QUrl VideoInfo::getBURL()
{
    return videoBAddress;
}

QUrl VideoInfo::getRURL()
{
    return videoRAddress;
}

int VideoInfo::getStatus()
{
    return status;
}

int VideoInfo::getVideoParts()
{
    return videoParts;
}

void VideoInfo::setVideo(QUrl addr)
{
    videoBAddress = addr;
    QStringList partlist = addr.toString().split('/', QString::SkipEmptyParts);
    foreach (QString part, partlist)
        if (part.contains("av"))
        {
            videobid = part;
            break;
        }
}

void VideoInfo::setPosition(int p)
{
    position = p;
}


void VideoInfo::start()
{
    if (status == URL_PARSED_OK)
        return;

    statusReset();
    buffer.open(QBuffer::WriteOnly);
    httpStart(videoBAddress);
}

void VideoInfo::httpStart(QUrl url)
{
    reply = qnam.get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()),
            this, SLOT(httpFinished()));
    connect(reply, SIGNAL(readyRead()),
            this, SLOT(httpReadyRead()));
}

void VideoInfo::httpReadyRead()
{
    // this slot gets called every time the QNetworkReply has new data.
    buffer.write(reply->readAll());
}

void VideoInfo::httpFinished()
{
    buffer.close();

    // Aborted
    if (httpRequestAborted)
    {
        reply->deleteLater();
        status = URL_PARSED_NETWORK_BAD;
        return;
    }

    // Error Handling
    // Redirection
    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

    if (reply->error()) {
        // Network error or 404
        switch (reply->error())
        {
            case QNetworkReply::ContentNotFoundError:
            {
                status = URL_PARSED_VIDEO_NOT_FOUND;
                videoRealName = QString::fromUtf8("视频未找到");
                videoRAddress = videoRealName;
                break;
            }
            default:
            {
                status = URL_PARSED_NETWORK_BAD;
                videoRealName = QString::fromUtf8("网络错误");
                videoRAddress = videoRealName;
            }
        }
        reply->deleteLater();
        emit parseFinished(position);
        return;
    }
    // Redirection
    else if (!redirectionTarget.isNull()) {
        QUrl newUrl = reply->url().resolved(redirectionTarget.toUrl());
        reply->deleteLater();
        status = URL_PARSED_REDIRECTION;
        httpStart(newUrl);
        return;
    }

    if (!videocid.isEmpty())                                // Parse XML
    {
        if (!parseXML())
        {
            status = URL_PARSED_XML_BAD;
            videoRAddress = QString::fromUtf8("视频信息获取失败");
        }
        else
            status = URL_PARSED_OK;
        emit parseFinished(position);
    }
    else if (parseHTML())                                   // Parse html
    {
        interface.setUrl(INTERFACE_URL + videocid);
        buffer.buffer().clear();
        buffer.open(QBuffer::WriteOnly);
        reply->deleteLater();
        httpStart(interface);
        return;
    }
    else
    {
        status = URL_PARSED_HTML_BAD;
        videoRealName = QString::fromUtf8("视频信息获取失败");
        videoRAddress = videoRealName;
        emit parseFinished(position);
    }

    // Clean up
    reply->deleteLater();
    reply = 0;
}


bool VideoInfo::parseHTML()
{
    int pos;
    int p1 = buffer.data().indexOf(PLAYER_STR);
    int p2 = buffer.data().indexOf(PLAYER_STR2);
    if (p1 == -1 && p2 == -1)
    {
        status = URL_PARSED_VIDEO_NOT_FOUND;
        return false;
    }
    else
    {
        if (p1 == -1) pos = p2 + PLAYER_STR2.length();
        else pos = p1 + PLAYER_STR.length();
        QChar t = buffer.data().at(pos);
        while (t.isDigit())
        {
            videocid.append(t);
            pos++;
            t = buffer.data().at(pos);
        }
    }

    pos = buffer.data().indexOf(NAME_STR);
    if (pos == -1)
    {
        status = URL_PARSED_VIDEO_NOT_FOUND;
        return false;
    }
    else
    {
        pos += NAME_STR.length();
        char t = buffer.data().at(pos);
        QByteArray b;
        while (t != '\"')
        {
            b.append(t);
            pos++;
            t = buffer.data().at(pos);
        }
        videoRealName = QString::fromUtf8(b.data());
    }
    return true;
}

bool VideoInfo::parseXML()
{
    QDomDocument doc("bin");
    if (doc.setContent(buffer.buffer()))
    {
        QDomNodeList durl = doc.elementsByTagName("url");
        // TODO: Parsing URL, remember "BACKUP_URLS"
        for (unsigned int i = 0; i < durl.length(); i++)
        {
            QDomNode url = durl.at(i);
            if (url.parentNode().nodeName() == "durl")
                videoParts++;
        }
        videoRAddress = doc.elementsByTagName("url").at(0).toElement().text();
        return true;
    }
    else return false;
}

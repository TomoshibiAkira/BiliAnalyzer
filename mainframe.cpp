#include "mainframe.h"
#include "ui_mainframe.h"
#include "aboutwindow.h"
#include "status.h"
#include "vptr.h"

#include <QUrl>
#include <QRegExp>
#include <QMimeData>
#include <QDataStream>

const QRegExp URL_REGEX = QRegExp("(http://)?(www)?\\.bilibili\\.tv/+video/+av\\d+/?");
const QRegExpValidator URL_PARSE(URL_REGEX, 0);

MainFrame::MainFrame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainFrame)
{
    videolist.clear();

    ui->setupUi(this);
    this->setWindowTitle(QString("Bilibili Video Parser   Ver ") + VERSION_NUMBER);

    connect (ui->analyzeButton, SIGNAL(clicked()), this, SLOT(startParse()));
    connect (ui->copyAllButton, SIGNAL(clicked()), this, SLOT(copyAll()));
    connect (ui->copyButton, SIGNAL(clicked()), this, SLOT(copy()));
    connect (ui->aboutButton, SIGNAL(clicked()), this, SLOT(about()));
    connect (ui->allClearButton, SIGNAL(clicked()), this, SLOT(reset()));
    connect (ui->appendButton, SIGNAL(clicked()), this, SLOT(listAppend()));

    connect (this, SIGNAL(videoInfoChanged(QModelIndex,QModelIndex)),
             &model, SIGNAL(dataChanged(QModelIndex,QModelIndex)));

    model.setList(videolist);
    ui->result->setModel(&model);
    selection = ui->result->selectionModel();
    ui->result->horizontalHeader()->setMovable(false);
    ui->result->horizontalHeader()->setClickable(false);
    ui->result->horizontalHeader()->resizeSection(0, 30);
    ui->result->horizontalHeader()->resizeSection(1, 80);
    ui->result->horizontalHeader()->resizeSection(2, 250);
    ui->result->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
    ui->result->horizontalHeader()->setResizeMode(1, QHeaderView::Fixed);

    clipboard = QApplication::clipboard();
}

void MainFrame::reset()
{
    if (videolist.size() == 0) return;
    foreach (VideoInfo* video, videolist)
        video->deleteLater();

    model.removeRows(0, videolist.size());
    videolist.clear();    
    ui->textInput->clear();
}

void MainFrame::listAppend()
{
    QStringList addrlist = ui->textInput->text().split(";",QString::SkipEmptyParts);
    int pos = 0, videoP = videolist.size();
    bool duplicate;

    foreach (QString addr, addrlist)
    {
        addr = addr.trimmed();
        QUrl url(addr);
        if (URL_PARSE.validate(addr, pos) != QRegExpValidator::Invalid)
        {
            if (addr[0] != 'h') url.setUrl("http://" + addr);
        }
        else if (url.isValid())
        {
            QRegExp exp("av\\d+/?");
            if (!exp.exactMatch(addr))
                continue;
            addr = "http://www.bilibili.tv/video/" + addr + "/";
            url.setUrl(addr);
        }
        else continue;

        duplicate = false;
        foreach (VideoInfo* video, videolist)
            if (video->getBURL().toString() == url.toString())
            {
                duplicate = true;
                break;
            }
        if (duplicate) continue;

        VideoInfo* video = new VideoInfo(url);
        video->setPosition(videoP++);
        videolist.append(video);

        model.insertRows(model.rowCount(), 1);
        QModelIndex index = model.index(videoP - 1, 0);

        QVariant v = VPtr<VideoInfo>::asQVariant(video);
        model.setData(index, v);
    }
    ui->textInput->clear();
}

void MainFrame::startParse()
{
    if (videolist.size() == 0) return;

    count = videolist.size();
    foreach (VideoInfo* video, videolist)
    {
        if (video->getStatus() == URL_NOT_PARSED)
        {
            connect (this, SIGNAL(getRealURL()), video, SLOT(start()));
            connect (video, SIGNAL(parseFinished(int)), this, SLOT(videoParsed(int)));
        }
    }
    changeButtonState(false);
    emit getRealURL();
}

void MainFrame::videoParsed(int p)
{
    count--;
    QModelIndex index = model.index(p, 0);
    if (count == 0) changeButtonState(true);
    ui->result->resizeColumnToContents(2);
    emit videoInfoChanged(index, index.sibling(p, 2));
}

void MainFrame::copyAll()
{
    cdata.clear();
    foreach (VideoInfo* video, videolist)
        cdata += video->getRURL().toString() + '\n';
    clipboard->clear();
    clipboard->setText(cdata);
}

void MainFrame::copy()
{
    cdata.clear();
    QModelIndexList rows = selection->selectedRows();
    foreach (QModelIndex row, rows)
    {
        int p = row.row();
        cdata += videolist.at(p)->getRURL().toString() + '\n';
    }
    clipboard->clear();
    clipboard->setText(cdata);
}

void MainFrame::about()
{
    aboutWindow* about = new aboutWindow();
    about->show();
}

void MainFrame::changeButtonState(bool b)
{
    ui->textInput->setEnabled(b);
    ui->analyzeButton->setEnabled(b);
    ui->allClearButton->setEnabled(b);
    ui->appendButton->setEnabled(b);
    ui->copyAllButton->setEnabled(b);
    ui->copyButton->setEnabled(b);
}

MainFrame::~MainFrame()
{
    delete ui;
}

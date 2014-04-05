#include "aboutwindow.h"
#include "ui_aboutwindow.h"
#include <QUrl>
#include <QMovie>
#include <QDesktopServices>

aboutWindow::aboutWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::aboutWindow)
{
    ui->setupUi(this);

    QMovie *doge100 = new QMovie(":/doge/doge100.gif");
    ui->doge->setMovie(doge100);
    doge100->start();

    ui->about->setText("Bilibili Video Parser Ver " + VERSION_NUMBER + "\n\n"
                       + "Created by El Mirador - just for some fun.\n\n"

                       + "Input: Just type any avXXXXXX\n" +
                       + "or a full bilibili URL\n\n"

                       + "Happy Using!\n\n"
                       + "Sina Weibo: @El_Mirador | Many THX! ");

    setFixedSize(sizeHint());

    connect (ui->dogeButton, SIGNAL(clicked()), this, SLOT(dogeURL()));

}

void aboutWindow::dogeURL()
{
    QDesktopServices::openUrl(QUrl("http://knowyourmeme.com/memes/doge"));
}

aboutWindow::~aboutWindow()
{
    delete ui;
}

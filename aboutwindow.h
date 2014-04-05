#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include <QWidget>

const QString VERSION_NUMBER("0.12");

namespace Ui {
class aboutWindow;
}

class aboutWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit aboutWindow(QWidget *parent = 0);
    ~aboutWindow();

private slots:
    void dogeURL();
    
private:
    Ui::aboutWindow *ui;
};

#endif // ABOUTWINDOW_H

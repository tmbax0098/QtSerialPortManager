#include "tools.h"

#include <QWindow>

Tools::Tools(QObject *parent) : QObject(parent)
{
    btnSize.setWidth(25);
    btnSize.setHeight(25);
}

QSize Tools::getBtnSize() const
{
    return btnSize;
}

void Tools::setBtnSize(const QSize &value)
{
    btnSize = value;
}

void Tools::setWindowIcon(QWindow* win, QString icnAddr)
{
    QPixmap pic(icnAddr);
    win->setIcon(QIcon(pic));
    //win->setIc(btnSize);
}

void Tools::setButtonIcon(QPushButton* btn, QString icnAddr)
{
    QPixmap pic(icnAddr);
    btn->setIcon(QIcon(pic));
    btn->setIconSize(btnSize);
}

void Tools::setProgressBarValue(QProgressBar *prog, int val)
{
    if(val>=prog->minimum() && val<=prog->maximum()){
        prog->setValue(val);
    }
}

void Tools::setLabelValue(QLabel *lbl, QString txt)
{
    lbl->setText(txt);
}

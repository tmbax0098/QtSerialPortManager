#ifndef TOOLS_H
#define TOOLS_H

#include <QLabel>
#include <QObject>
#include <QProgressBar>
#include <QPushButton>
#include <QWidget>

class Tools : public QObject
{
    Q_OBJECT
public:
    explicit Tools(QObject *parent = nullptr);

    QSize getBtnSize() const;
    void setBtnSize(const QSize &value);

    void setWindowIcon(QWindow* win , QString icnAddr);
    void setButtonIcon(QPushButton* btn , QString icnAddr);
    void setProgressBarValue(QProgressBar* prog , int val = 0);
    void setLabelValue(QLabel* lbl , QString txt="");

signals:

public slots:


private :
    QSize btnSize;
};

#endif // TOOLS_H

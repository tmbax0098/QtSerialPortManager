#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "portmanager.h"
#include "tools.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnCleanRecieve_clicked();
    void on_btnClosePort_clicked();
    void on_btnOpenPort_clicked();
    void ManageStatus( bool status = false);
    void ManageNewPacket(QString packet);
    void on_btnSendPacket_clicked();
    void on_btnRefreshPortList_clicked();

    void on_comReadStyle_currentIndexChanged(const QString &arg1);

    void on_comWorkStyle_currentIndexChanged(const QString &arg1);

    void on_comConvertTo_currentIndexChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    PortManager pm;
    Tools tool;
};

#endif // MAINWINDOW_H

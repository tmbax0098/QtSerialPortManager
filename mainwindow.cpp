#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    ui->comPortList->addItems(pm.getPortList());
    ui->comDataBit->addItems(pm.getDataBitList());
    ui->comParity->addItems(pm.getParityList());
    ui->comBaudrate->addItems(pm.getBaudrateList());
    ui->comStopBit->addItems(pm.getStopBitList());
    ui->comBaudrate->setValidator( new QIntValidator(0, 9999999, this) );
    ui->leDelay->setValidator(new QIntValidator(0,100000));

    tool.setButtonIcon(ui->btnRefreshPortList,":/refresh.png");

    QObject::connect(&pm,SIGNAL(StatusChanged(bool)) , this,SLOT(ManageStatus(bool)));
    QObject::connect(&pm,SIGNAL(NewPacketRecived(QString)) , this,SLOT(ManageNewPacket(QString)));

    //setWindowIcon(QIcon(QPixmap(":/refresh.png")));
    //tool.setWindowIcon(,":/serialport.png");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnCleanRecieve_clicked()
{
    ui->txtEditRecv->setText("");
}

void MainWindow::on_btnClosePort_clicked()
{
    pm.stop();
}

void MainWindow::on_btnOpenPort_clicked()
{
    pm.setPortName(ui->comPortList->currentText());
    pm.setParity(ui->comParity->currentText());
    pm.setDataBit(ui->comDataBit->currentText());
    pm.setBaudrate(ui->comBaudrate->currentText());
    if(ui->comConvertTo->currentText() == "Hex")
        pm.setConvertRole(PortManager::RoleHex);
    else if(ui->comConvertTo->currentText() == "Decimal") {
        pm.setConvertRole(PortManager::RoleDecimal);
    }else{
        pm.setConvertRole(PortManager::RoleString);
    }
    if(ui->comWorkStyle->currentIndex()== 0){
        pm.setWorkStyle(PortManager::ReadAfterWrite);
    }else{
        pm.setWorkStyle(PortManager::AlwaysRead);
    }
    pm.setDelayTime(ui->leDelay->text().toInt());
    ui->btnOpenPort->setEnabled(false);
    pm.start();
}

void MainWindow::ManageStatus(bool status)
{
    if(status == true){
        ui->comParity->setEnabled(false);
        ui->comDataBit->setEnabled(false);
        ui->comBaudrate->setEnabled(false);
        ui->comPortList->setEnabled(false);
        ui->btnOpenPort->setEnabled(false);
        ui->comConvertTo->setEnabled(false);
        ui->comWorkStyle->setEnabled(false);
        ui->leDelay->setEnabled(false);
        ui->comStopBit->setEnabled(false);
        ui->comSendSort->setEnabled(false);
        ui->btnRefreshPortList->setEnabled(false);

        ui->comReadStyle->setEnabled(false);
        ui->comWriteLine->setEnabled(false);
        //leDelay
        //comWorkStyle
        //comConvertTo

        ui->btnClosePort->setEnabled(true);
    }else{
        ui->comParity->setEnabled(true);
        ui->comDataBit->setEnabled(true);
        ui->comBaudrate->setEnabled(true);
        ui->comPortList->setEnabled(true);
        ui->btnOpenPort->setEnabled(true);
        ui->comConvertTo->setEnabled(true);
        ui->comWorkStyle->setEnabled(true);
        ui->leDelay->setEnabled(true);
        ui->comStopBit->setEnabled(true);
        ui->comSendSort->setEnabled(true);
        ui->btnRefreshPortList->setEnabled(true);

        ui->comReadStyle->setEnabled(true);
        ui->comWriteLine->setEnabled(true);

        ui->btnClosePort->setEnabled(false);
    }
}

void MainWindow::ManageNewPacket(QString packet)
{
    //    if(pm.getWorkStyle() == PortManager::WorkStyle::ReadAfterWrite){
    //        ui->txtEditRecv->setText(packet);
    //    }else{
    //        ui->txtEditRecv->append(packet);
    //    }
    ui->txtEditRecv->append(packet);
}

void MainWindow::on_btnSendPacket_clicked()
{
    QString value = ui->lineEditSend->text();
    QByteArray cmd;
    QString role = ui->comSendSort->currentText();
    if(role == "Hex"){
        cmd = QByteArray::fromHex(value.toUtf8());
    }else if(role == "Decimal"){
        QStringList list = value.split(" ");
        foreach (QString item, list) {
            int val = item.toInt();
            //qDebug()<<"val : "<<val<<endl;
            if(val >= 0 && val < 256){
                cmd.append(val);
            }
        }
    }else{
        cmd = value.toUtf8();
    }
    if(pm.getWorkStyle() == PortManager::WorkStyle::AlwaysRead){
        pm.write(cmd);
    }else{
        pm.writeAndRead(cmd);
    }

}

void MainWindow::on_btnRefreshPortList_clicked()
{
    ui->comPortList->clear();
    ui->comPortList->addItems(pm.getPortList());
}

void MainWindow::on_comReadStyle_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "Yes"){
        pm.setReadLine(PortManager::RoleYes);
    }else{
        pm.setReadLine(PortManager::RoleNo);
    }
}

void MainWindow::on_comWorkStyle_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "Read after write"){
        pm.setWorkStyle(PortManager::ReadAfterWrite);
    }else if(arg1 == "Always read"){
        pm.setWorkStyle(PortManager::AlwaysRead);
    }
}

void MainWindow::on_comConvertTo_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "Hex"){pm.setConvertRole(PortManager::RoleHex);}
    else if(arg1 == "Decimal"){pm.setConvertRole(PortManager::RoleDecimal);}
    else if(arg1 == "String"){pm.setConvertRole(PortManager::RoleString);}
}

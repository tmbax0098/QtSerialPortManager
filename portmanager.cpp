#include "portmanager.h"
#include<QSerialPort>
#include<QSerialPortInfo>
#include<QDebug>
#include <QThread>
#include <QMutex>

PortManager::PortManager(QObject *parent) : QThread(parent)
{

    delay = 50;
    this->setReadLine(RoleNo);
    this->setWriteLine(RoleNo);

}

QStringList PortManager::getPortList()
{
    QStringList list;
    foreach (QSerialPortInfo item, QSerialPortInfo::availablePorts()) {
        list.append(item.portName());
    }
    return list;
}

QStringList PortManager::getDataBitList()
{
    QStringList list;
    for (int var = 8; var > 4; --var) {
        list.append("Data"+ QString::number( var));
    }
    return  list;
}

QStringList PortManager::getParityList()
{
    QStringList list;
    list.append("NoParity");
    list.append("OddParity");
    list.append("EvenParity");
    list.append("MarkParity");
    list.append("SpaceParity");
    list.append("UnknownParity");
    return list;
}

QStringList PortManager::getBaudrateList()
{
    QStringList list;
    list.append("115200");
    list.append("57600");
    list.append("38400");
    list.append("19200");
    list.append("9600");
    list.append("4800");
    list.append("2400");
    list.append("1200");
    return  list;
}

QStringList PortManager::getStopBitList()
{
    QStringList list;
    list.append("OneStop");
    list.append("TwoStop");
    list.append("OneAndHalfStop");
    list.append("UnknownStopBits");
    //QSerialPort::StopBits::UnknownStopBits
    return list;
}

ulong PortManager::getDelaytime()
{
    return this->delay;
}

void PortManager::setDataBit(QString value)
{
    if(value == "Data8"){
        sp.setDataBits(QSerialPort::Data8);
    }else if(value == "Data7"){
        sp.setDataBits(QSerialPort::Data7);
    }else if(value == "Data6"){
        sp.setDataBits(QSerialPort::Data6);
    }else if(value == "Data5"){
        sp.setDataBits(QSerialPort::Data5);
    }else if(value == "UnknownDataBits"){
        sp.setDataBits(QSerialPort::UnknownDataBits);
    }
}

void PortManager::setBaudrate(QString value)
{
    sp.setBaudRate(value.toInt());
}

void PortManager::setParity(QString value)
{
    if(value == "NoParity"){ sp.setParity(QSerialPort::Parity::NoParity);}
    else if(value == "OddParity"){sp.setParity(QSerialPort::Parity::OddParity);}
    else if(value == "EvenParity"){sp.setParity(QSerialPort::Parity::EvenParity);}
    else if(value == "MarkParity"){sp.setParity(QSerialPort::Parity::MarkParity);}
    else if(value == "SpaceParity"){sp.setParity(QSerialPort::Parity::SpaceParity);}
    else if(value == "UnknownParity"){sp.setParity(QSerialPort::Parity::UnknownParity);}
}

void PortManager::setPortName(QString value)
{
    if(value != ""){
        sp.setPortName(value);
    }
}

void PortManager::setStopBit(QString value)
{

    if(value =="OneStop"){
        sp.setStopBits(QSerialPort::OneStop);
    }
    else if(value =="TwoStop"){
        sp.setStopBits(QSerialPort::TwoStop);
    }
    else if(value =="OneAndHalfStop"){
        sp.setStopBits(QSerialPort::OneAndHalfStop);
    }
    else if(value =="UnknownStopBits"){
        sp.setStopBits(QSerialPort::UnknownStopBits);
    }
}

void PortManager::stop()
{
    if(sp.isOpen() == true){
        sp.close();
        if(roleWork == AlwaysRead){
            QObject::disconnect(&sp,SIGNAL(readyRead()),this,SLOT(ReadAllByte()));
        }
    }
    emit StatusChanged(false);
}

void PortManager::start()
{
    if(sp.open(QIODevice::ReadWrite)){
        if(roleWork == AlwaysRead){
            QObject::connect(&sp,SIGNAL(readyRead()),this,SLOT(ReadAllByte()));
        }
    }
    emit StatusChanged(sp.isOpen());
}

void PortManager::write(QByteArray value)
{
    if(sp.isOpen()){
        if(writeLine == RoleNo){
            sp.write(value);
        }else if(writeLine == RoleYes){
            value.append('\n');
            sp.write(value);
        }
    }
}

void PortManager::writeAndRead(QByteArray value)
{
    sendArray.clear();
    sendArray = value;
    run();
}

void PortManager::setDelayTime(ulong value)
{
    delay = value;
}

void PortManager::setWorkStyle(PortManager::WorkStyle style)
{
    roleWork = style;
}

void PortManager::setConvertRole(PortManager::ConvertStyle role)
{
    this->role =role;
}

void PortManager::ReadAllByte()
{
    if(sp.isOpen()){
        if (sp.waitForReadyRead(delay)) {
            if(readLine == RoleNo){
                recvArray+=sp.readAll();
                while (sp.waitForReadyRead(10))
                    recvArray += sp.readAll();
            }else if(readLine == RoleYes){
                recvArray+=sp.readLine();
            }
            buildPacket();
        }
    }
}

void PortManager::buildPacket()
{
    QString packet = "";
    if(role == RoleHex){
        packet =QString(recvArray.toHex());
    }else if(role == RoleDecimal){
        int i = 0;
        for(;i < recvArray.length() -1 ;i++){
            packet += QString::number( recvArray.at(i)) + " ";
        }
        packet +=QString ::number(recvArray[i]);
    }else if(role == RoleString){
        packet = recvArray.data();
    }
    //qDebug()<<"packet : "<<packet<<endl;
    emit NewPacketRecived(packet);
    recvArray.clear();
}

void PortManager::setWriteLine(const Answer &value)
{
    writeLine = value;
}

void PortManager::setReadLine(const Answer &value)
{
    readLine = value;
}

void PortManager::run()
{
    if(sp.isOpen()){
        recvArray.clear();
        sp.clear(QSerialPort::Direction::AllDirections);
        if(writeLine == RoleYes){
            //write line
            sendArray.append('\n');
            sp.write(sendArray);
        }else if(writeLine == RoleNo){
            sp.write(sendArray);
        }

        if(sp.waitForBytesWritten(delay)){
            if(sp.waitForReadyRead(delay)){
                if(readLine == RoleNo){
                    recvArray = sp.readAll();
                    while (sp.waitForReadyRead(10))
                        recvArray += sp.readAll();
                }else if(readLine == RoleYes){
                    recvArray = sp.readLine();
                }
                buildPacket();
            }
        }

    }
}

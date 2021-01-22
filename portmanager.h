#ifndef PORTMANAGER_H
#define PORTMANAGER_H

#include <QObject>
//#include <QThread>
#include <QSerialPort>
#include <QThread>

class PortManager : public QThread {

    Q_OBJECT
public:
    explicit PortManager(QObject *parent = nullptr);
    QStringList getPortList();
    QStringList getDataBitList();
    QStringList getParityList();
    QStringList getBaudrateList();
    QStringList getStopBitList();
    ulong getDelaytime();

    void setDataBit(QString value = "Data8");
    void setBaudrate(QString value = "9600");
    void setParity(QString value = "");
    void setPortName(QString value = "");
    void setStopBit(QString value = "");
    void stop();
    void start();
    void write(QByteArray value);

    void writeAndRead(QByteArray value);
    void setDelayTime(ulong value = 50);



    enum ConvertStyle {
        RoleDecimal = 0,
        RoleHex = 1,
        RoleString
    };
    enum WorkStyle{
        ReadAfterWrite = 0,
        AlwaysRead
    };

    enum Answer{
        RoleNo = 0,
        RoleYes
    };


    void setWorkStyle(WorkStyle style = ReadAfterWrite);
    WorkStyle getWorkStyle(){

        if(roleWork == ReadAfterWrite){
            return  ReadAfterWrite;
        }
        return  AlwaysRead;

    }
    void setConvertRole(PortManager::ConvertStyle role);


    Answer getReadLine() const
    {
        return readLine;
    }
    void setReadLine(const Answer &value);

    Answer getWriteLine() const
    {
        return writeLine;
    }
    void setWriteLine(const Answer &value);

private slots:
    void ReadAllByte();
    void buildPacket();

signals:
    void StatusChanged(bool value = false);
    void NewPacketRecived(QString packet);

public slots:

    // QThread interface
protected:
    QSerialPort sp;
    QByteArray recvArray;
    QByteArray sendArray;
    WorkStyle roleWork;
    int role;
    Answer readLine;
    Answer writeLine;
    ulong delay;//mili second delay time
    QThread th;
    void run();
};

#endif // PORTMANAGER_H

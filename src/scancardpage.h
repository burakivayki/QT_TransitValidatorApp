#ifndef SCANCARDPAGE_H
#define SCANCARDPAGE_H

#include "packetgenerator.h"
#include <QWidget>
#include <QSerialPort>
#include "serialprocessor.h"

#define SERIAL_READER_PORT "/dev/ttymxc3"
#define BAUD_RATE_INT 921600
#define BAUD_RATE_STR "921600"
#define PROGRAM_COMMAND "lpccmd"
#define ARGUEMENT_1 "-b"
#define ARGUEMENT_2 "--getinf"
#define HEX_STRING "02 09 00 3D DF 0C 00 E5 03"
#define GIVEN_INS 0X10
#define GIVEN_DATA "AA BB CC"

#define STX_VALUE 0x02
#define PCB_VALUE 0x00
#define ETX_VALUE 0x03


namespace Ui{
class ScanCardPage;
}

class ScanCardPage : public QWidget
{
    Q_OBJECT

public:
    explicit ScanCardPage(QWidget *parent = nullptr); //ÇĞREN
    ~ScanCardPage();

    enum class PacketCommandType {
        GetInfo,
        RfReset,
        RfOnOff,
        RfIsPresent,
        RfPoll,
    };
    enum class ScopeType {
        Do,
        Set,
        Get,
        Nak,
    };

    void cardPageUI();

signals:
    void returnToMainPage();

private slots:
    void on_exitButton_clicked();
    void readSerialData();
    void on_refreshBut_clicked();
    void appendToTerminal(const QString &message);
    void on_getInfBut_clicked();
    void on_rfResetBut_clicked();

private:
    Ui::ScanCardPage*ui;
    QSerialPort*serialPort;
    QByteArray buffer;
    QByteArray rec_buffer;
    SerialProcessor *processor;
    QByteArray currentPacketToSend;
    void buildAndSendPacket( ScopeType Scope, PacketCommandType cmdType);
};

#endif//SCANCARDPAGE_H

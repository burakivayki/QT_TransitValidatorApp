#include "scancardpage.h"
#include "ui_scancardpage.h"
#include "packetgenerator.h"
#include <QRandomGenerator>

#include <QProcess>
#include <QString>
#include <QStringList>
#include <QDebug>

void ScanCardPage::cardPageUI(){
    this->setWindowTitle("ScanCard");
    ui->pageInfo->setText(this->windowTitle());
    ui->terminalBrowser->setText("");
    ui->exitButton->setText("Return to the main page");
    ui->getInfBut->setText("GetInf Command");
    ui->ter1->setText("---");
    ui->ter2->setText("---");
    ui->ter3->setText("---");
    ui->ter4->setText("---");
    ui->refreshBut->setText("Refresh the terminal");
    ui->hexLabel->setText(""); ui->hexLabel->setStyleSheet("color:black");
}

ScanCardPage::ScanCardPage(QWidget*parent): //BU FONKSIYON NASI ÇALIŞTI
    QWidget(parent),ui(new Ui::ScanCardPage){

    ui->setupUi(this);

    processor = new SerialProcessor(this);
    connect(processor, &SerialProcessor::messageReady, this, &ScanCardPage::appendToTerminal);

    serialPort = new QSerialPort(this);
    serialPort->setPortName(SERIAL_READER_PORT);
    serialPort->setBaudRate(BAUD_RATE_INT);

    if(serialPort->open(QIODevice::ReadWrite)){
        connect(serialPort, &QSerialPort::readyRead, this, &ScanCardPage::readSerialData);
        ui->outputLabel->setText("Connected to port succesfully.");
        ui->outputLabel->setStyleSheet("color:green;");
    } else{
        ui->outputLabel->setText("Could not connect to port:"+serialPort->errorString());
        ui->outputLabel->setStyleSheet("color:red;");
    }
}

void ScanCardPage::buildAndSendPacket(PacketCommandType cmdType){
    ui->terminalBrowser->setText("");
    quint8 stx = STX_VALUE;
    quint8 etx = ETX_VALUE;
    quint8 pcb = PCB_VALUE;

    quint8 ins = 0;
    quint16 tag = 0;
    QByteArray value;

    switch(cmdType){
        case PacketCommandType::GetInfo:
            ins = 0x3D;
            tag = 0xDF0C;
            //VALUE BOŞ
            break;
    }

    quint8 len = static_cast<quint8>(value.size());

    PacketGenerator generator;
    currentPacketToSend = generator.createPacket(stx, pcb, ins, etx, tag, len, value);

    QString packetText = currentPacketToSend.toHex(' ').toUpper();

    if (serialPort->isOpen() && serialPort->isWritable()){
        serialPort->write(currentPacketToSend);
        ui->terminalBrowser->append("Sent packet: " + packetText + "\n");
    } else {
        ui->terminalBrowser->append("Error: The port is closed or unwritable");
    }
}

void ScanCardPage::readSerialData(){ //READER'A AL - SERI PORTUN TAMAMINI
    processor->processData(serialPort->readAll());
}

void ScanCardPage::on_refreshBut_clicked(){
    ui->terminalBrowser->setText("");
    ui->hexLabel->setText("");
}

void ScanCardPage::on_getInfBut_clicked()
{
    buildAndSendPacket(PacketCommandType::GetInfo);
}

void ScanCardPage::appendToTerminal(const QString &message){
    ui->terminalBrowser->append(message);
}

void ScanCardPage::on_exitButton_clicked(){
    emit returnToMainPage();
}

ScanCardPage::~ScanCardPage(){

    if(serialPort->isOpen()){
        serialPort->close();
    }
    delete ui;
}

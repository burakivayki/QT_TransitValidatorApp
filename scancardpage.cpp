#include "scancardpage.h"
#include "ui_scancardpage.h"
#include <QProcess>
#include <QString>
#include <QStringList>

void ScanCardPage::cardPageUI(){
    this->setWindowTitle("ScanCard");

    ui->pageInfo->setText(this->windowTitle());
    ui->exitButton->setText("Return to the main page");
    ui->getInfBut->setText("GetInf");
    ui->terminalBrowser->setText("");
    ui->ter1->setText("SendTX");
    ui->ter2->setText("---");
    ui->ter3->setText("---");
    ui->ter4->setText("---");
    ui->ter5->setText("Refresh the terminal");
    ui->statusLabel->setText("");ui->statusLabel->setStyleSheet("color:black");

}

ScanCardPage::ScanCardPage(QWidget*parent):
    QWidget(parent),ui(new Ui::ScanCardPage){

    ui->setupUi(this);

    serialPort = new QSerialPort(this);
    serialPort->setPortName("/dev/ttymxc3");
    serialPort->setBaudRate(921600);

    if(serialPort->open(QIODevice::ReadWrite)){
        connect(serialPort,&QSerialPort::readyRead,this,&ScanCardPage::readSerialData);
        ui->outputLabel->setText("Connected to port succesfully.");
        ui->outputLabel->setStyleSheet("color:green;");
    }
    else{
        ui->outputLabel->setText("Could not connect to port:"+serialPort->errorString());
        ui->outputLabel->setStyleSheet("color:red;");
    }
}

ScanCardPage::~ScanCardPage(){

    if(serialPort->isOpen()){
        serialPort->close();
    }
    delete ui;
}

void ScanCardPage::on_exitButton_clicked(){
    emit returnToMainPage();
}

void ScanCardPage::on_getInfBut_clicked(){

    ui->terminalBrowser->setText("");
    QProcess process;
    QString program="lpccmd";
    QStringList arguments;
    arguments<<"-b"<<"921600"<<"--getinf"<<"/dev/ttymxc3";

    process.start(program,arguments);
    if(!process.waitForStarted(2000)){
        ui->terminalBrowser->setText("Terminal Error: "+process.errorString());
        return;
    }

    process.waitForFinished(3000);
    QString output=process.readAllStandardOutput();
    QString errorOutput=process.readAllStandardError();
    QString combinedOutput;

    if(!output.isEmpty()){
        combinedOutput+="(ExpectedOutput):\n"+output+"\n";
    }

    if(!errorOutput.isEmpty()){
        combinedOutput+="(ErrorOutput):\n"+errorOutput+"\n";
    }

    if(combinedOutput.isEmpty()){
        combinedOutput="Command has been runned but could not get any output.Output code:(ExitCode):"
                        + QString::number(process.exitCode());
    }

    ui->terminalBrowser->setText(combinedOutput);
}

void ScanCardPage::on_ter1_clicked(){

    ui->terminalBrowser->setText("");
    QString hexString="02 09 00 3D DF 0C 00 E5 03";
    QByteArray dataToSend=QByteArray::fromHex(hexString.toUtf8());

    if(serialPort->isOpen() && serialPort->isWritable()){
        serialPort->write(dataToSend);
        ui->terminalBrowser->append("Sent: "+dataToSend.toHex(' ').toUpper());
    }
    else{
        ui->terminalBrowser->append("Error: The port is closed or unwritable.");
    }
}

void ScanCardPage::readSerialData(){

    rec_buffer.append(serialPort->readAll());
    int startIndex=-1;

    for(int i = 0; i<rec_buffer.size(); i++){
        if(rec_buffer[i] == (char)0x02||rec_buffer[i] == (char)0x00){
            startIndex=i;
            break;
        }
    }
    if(startIndex == -1){
        rec_buffer.clear();
        return;
    }
    if(startIndex > 0){
        rec_buffer.remove(0,startIndex);
    }
    if(rec_buffer.size() < 2) return;

    qint8 len1 = static_cast<quint8>(rec_buffer[1]);

    qint32 packetLength = 0;
    int lenSize = 0;

    if(len1 < 0x80){
        packetLength = len1;
        lenSize=1;
    }
    else if(len1 == 0x81){
        if(rec_buffer.size() < 3)return;
        packetLength=static_cast<quint8>(rec_buffer[2]);
        lenSize=2;
    }
    else if(len1 == 0x82){
        if(rec_buffer.size() < 4)return;
        quint8 len2=static_cast<quint8>(rec_buffer[2]);
        quint8 len3=static_cast<quint8>(rec_buffer[3]);
        packetLength=(len2 << 8) | len3;
        lenSize=3;
    }
    else {
        rec_buffer.remove(0,1);
        return;
    }

    if(rec_buffer.size() < (int)packetLength)return;

    QByteArray packet = rec_buffer.left(packetLength);
    rec_buffer.remove(0,packetLength);
    if(packet.endsWith(0x03)){
        quint8 stx = static_cast<quint8>(packet[0]);
        bool isValid = true;
    if(stx == 0x02){
        quint8 calcLRC=0;
        for(int i = 0; i<packetLength - 2; ++i){
        calcLRC ^= static_cast<quint8>(packet[i]);
        }
    quint8 receivedLRC = static_cast<quint8>(packet[packetLength-2]);
        if(calcLRC != receivedLRC){
            isValid = false;
        ui->terminalBrowser->append("\nError: LRC discrepancy");
        }
    }

    if(isValid){
        int headerSize = 1+lenSize+1+1;
        int footerSize = 2;
        int dataSize = packetLength-headerSize-footerSize;
        quint8 ins = static_cast<quint8>(packet[1+lenSize+1]);
        QByteArray dataField;
        if(dataSize > 0){
            dataField = packet.mid(headerSize,dataSize);
        }
    QString output=QString("\nINS:%1\nDATA:%2")
            .arg(QString::number(ins,16).toUpper().rightJustified(2,'0'))
            .arg(QString(dataField.toHex(' ').toUpper()));
    ui->terminalBrowser->append("\n[Packet Received]\n"+output);
    }
    }else{
        ui->terminalBrowser->append("\nError:ETX byte could not find");
    }
}

void ScanCardPage::on_ter5_clicked(){
    ui->terminalBrowser->setText("");
}




#include "scancardpage.h"
#include "ui_scancardpage.h"

#include <QProcess>
#include <QString>
#include <QStringList>

void ScanCardPage::cardPageUI(){
    this->setWindowTitle("ScanCard");
    ui->pageInfo->setText(this->windowTitle());
    ui->terminalBrowser->setText("");
    ui->exitButton->setText("Return to the main page");
    ui->getInfBut->setText("GetInf Command");
    ui->sendHexBut->setText("Send HEX");
    ui->ter2->setText("---");
    ui->ter3->setText("---");
    ui->ter4->setText("---");
    ui->refreshBut->setText("Refresh the terminal");
    ui->statusLabel->setText("");ui->statusLabel->setStyleSheet("color:black");
}

ScanCardPage::ScanCardPage(QWidget*parent):
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
    QString program = PROGRAM_COMMAND;
    QStringList arguments;
    arguments << ARGUEMENT_1 << BAUD_RATE_STR << ARGUEMENT_2 << SERIAL_READER_PORT;

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

void ScanCardPage::on_sendHexBut_clicked(){

    ui->terminalBrowser->setText("");
    QString hexString = HEX_STRING;
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
    processor->processData(serialPort->readAll());
}

void ScanCardPage::appendToTerminal(const QString &message){
    ui->terminalBrowser->append(message);
}

void ScanCardPage::on_refreshBut_clicked(){
    ui->terminalBrowser->setText("");
}




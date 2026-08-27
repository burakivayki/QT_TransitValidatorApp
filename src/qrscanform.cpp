#include "qrscanform.h"
#include "ui_qrscanform.h"
#include <QDebug>

void QrScanForm::qrUI(){
    this->setWindowTitle("QR Scanner");
    serialPort = new QSerialPort(this); //seri port nesnesi
    serialPort->setPortName(SERIAL_QR_PORT); //cihazın adresi
    serialPort->setBaudRate(BAUD_RATE); //hızı
    ui->exitButton->setText("Return to the main page");
    ui->qrOutputLabel->setText("---");
    ui->qrOutputLabel_2->setText("Please show QR to the scanner...");
    ui->infoLabel->setText(this->windowTitle());

    if(serialPort->open(QIODevice::ReadOnly)){ //sadece okuma modunda aç portu
        connect(serialPort, &QSerialPort::readyRead, this, &QrScanForm::readQrData);          //Port başarıyla açıldıysa, porta veri geldiğinde
        qDebug()<<"Connected to device succesfully. The application is running flawlessly";   //(readyRead) readQrData fonksiyonunu çalıştır
        ui->statusLabel->setText("Connected to device succesfully. The application is running flawlessly");
        ui->statusLabel->setStyleSheet("color: green;");
    }
    else {
        qDebug() << "Couldn't connect to port: " << serialPort->errorString();
        ui->statusLabel->setText("Couldn't connect to port: "+ serialPort->errorString());
        ui->statusLabel->setStyleSheet("color: red;");
    }
}

QrScanForm::QrScanForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QrScanForm)
{
    ui->setupUi(this);
}

QrScanForm::~QrScanForm()
{
    if(serialPort->isOpen()){ //program kapanırken port açıksa kapat
        serialPort->close();
    }
    delete ui;
}

void QrScanForm::readQrData(){
    buffer.append(serialPort->readAll()); //gelen tüm veriyi buffer'a ekle (append'le)
    ui->qrOutputLabel->setText(buffer);
}

void QrScanForm::on_exitButton_clicked()
{
    emit returnToMainPage();
}

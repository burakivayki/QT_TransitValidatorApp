#include "mainpage.h"
#include "ui_mainpage.h"
#include <QProcess>
#include <QDebug>
#include <QString>

MainPage::MainPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainPage)
{
    ui->setupUi(this);
    this->setWindowTitle("Main Page");

    QProcess process;
    process.start("ifconfig", QStringList() << "eth0");
    process.waitForFinished();

    QString output = process.readAllStandardOutput();
    QString errorOutput = process.readAllStandardError();

    if (!errorOutput.isEmpty()) {
        qDebug() << "Terminal Error:" << errorOutput;
    }
    qDebug() << "Terminal Output:" << output;

    QString ipAddress = "IP address of this device could not find";
    int startIndex = output.indexOf("inet addr:");

    if (startIndex != -1) {
        startIndex += 10;                               //"inet addr:" kısmını (10 karakter) atlayıp IP'nin başına geldi
        int endIndex = output.indexOf(' ', startIndex); //IP'den sonraki ilk boşluk

        if (endIndex != -1) {
            ipAddress = output.mid(startIndex, endIndex - startIndex);
        } else {
            // Eğer IP string'in en sonundaysa ve boşluk yoksa
            ipAddress = output.mid(startIndex);
        }
    }
    ui->ipLabel->setText("IP: " + ipAddress);

    ui->appName->setText("Application");
    ui->exitAppBut->setText("Exit");
    ui->qrWidgetBut->setText("Scan QR");
    ui->soundWidgetBut->setText("Sound Test");
    ui->tScreenWidgetBut->setText("Touch Screen Test");
    ui->scanCardBut->setText("Scan a Card");
    ui->button1->setText("Coming soon...");
    ui->button2->setText("Coming soon...");
}

MainPage::~MainPage()
{
    delete ui;
}

void MainPage::on_exitAppBut_clicked()
{
    system("dd if=/dev/zero of=/dev/fb0 2>/dev/null");
    //dd -> veri kopyalama komutu
    //if=/dev/zero -> input file olarak işletim sisteminin sonsuz 0 üreten mekanizması seçilir.
    //of=/dev/fb0 -> output file olarak cihazın görüntü belleğini (framebuffer) seçer ve ekrana siyah renk basmış olur.
    //2>/dev/null -> işlem sırasında veya sonunda terminale basılacak olası sistem hata mesajlarını çöpe yönlendirir ve terminal çıktısını temiz tutar.
    QApplication::quit(); //uygualamayı kapatır
}

void MainPage::on_qrWidgetBut_clicked()
{
    emit openQrScanner();
}

void MainPage::on_soundWidgetBut_clicked()
{
    emit openSoundTest();
}

void MainPage::on_tScreenWidgetBut_clicked()
{
    emit openTouchScreenTest();
}

void MainPage::on_scanCardBut_clicked()
{
    emit openScanCardPage();
}

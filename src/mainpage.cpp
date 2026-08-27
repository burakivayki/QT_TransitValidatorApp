#include "mainpage.h"
#include "ui_mainpage.h"

MainPage::MainPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainPage)
{
    ui->setupUi(this);
    this->setWindowTitle("Main Page");
    ui->exitAppBut->setText("Exit");
    ui->qrWidgetBut->setText("Scan QR");
    ui->soundWidgetBut->setText("Sound Test");
    ui->tScreenWidgetBut->setText("Touch Screen Test");
    ui->scanCardBut->setText("Scan a Card");
    ui->appName->setText("Application");
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

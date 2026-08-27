#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qrscanform.h"
#include "mainpage.h"
#include "soundtestpage.h"
#include "touchscreentest.h"
#include "scancardpage.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Main Page");

    MainPage *mainPage = new MainPage(this); //Sayfa nesnelerini oluşturma
    QrScanForm *qrPage = new QrScanForm(this);
    SoundTestPage *soundPage = new SoundTestPage(this);
    TouchScreenTest *tsPage = new TouchScreenTest(this);
    ScanCardPage *cardPage = new ScanCardPage(this);


    while(ui->stackedWidget->count() > 0) { //QStackedWidget'ı temizle (varsayılan sayfalar varsa)
        QWidget* widget = ui->stackedWidget->widget(0);
        ui->stackedWidget->removeWidget(widget);
        widget->deleteLater();
    }

    ui->stackedWidget->addWidget(mainPage); //Sayfaları ekle (Sırasıyla index 0 ve 1 olur)
    ui->stackedWidget->addWidget(qrPage);
    ui->stackedWidget->addWidget(soundPage);
    ui->stackedWidget->addWidget(tsPage);
    ui->stackedWidget->addWidget(cardPage);
//-----------------ALT SEKMELER-------------------
    connect(mainPage, &MainPage::openQrScanner, this, [=]() { //Ana sayfadaki butona basılınca QR sayfasına (index 1) geç
        qrPage->qrUI();
        ui->stackedWidget->setCurrentIndex(1);
    });

    connect(mainPage, &MainPage::openSoundTest, this, [=]() { //Ana sayfadaki butona basılınca Sound Test sayfasına (index 2) geç
        soundPage->soundTestUI();
        ui->stackedWidget->setCurrentIndex(2);
    });

    connect(mainPage, &MainPage::openTouchScreenTest, this, [=](){
        tsPage->tScreenUI();
        ui->stackedWidget->setCurrentIndex(3);
    });

    connect(mainPage, &MainPage::openScanCardPage, this, [=](){
        cardPage->cardPageUI();
        ui->stackedWidget->setCurrentIndex(4);
    });

//-------------ANA SAYFAYA DÖNMELER-------------
    connect(qrPage, &QrScanForm::returnToMainPage, this, [=]() { //QR sayfasındaki çıkışa basılınca Ana sayfaya (index 0) dön
        ui->stackedWidget->setCurrentIndex(0);
    });

    connect(soundPage, &SoundTestPage::returnToMainPage, this, [=]() { //Sound Test sayfasındaki çıkışa basılınca Ana sayfaya (index 0) dön
        ui->stackedWidget->setCurrentIndex(0);
    });

    connect(tsPage, &TouchScreenTest::returnToMainPage, this, [=](){
        ui->stackedWidget->setCurrentIndex(0);
    });

    connect(cardPage, &ScanCardPage::returnToMainPage, this, [=](){
        ui->stackedWidget->setCurrentIndex(0);
    });

//------------------------------------------------

    connect(ui->stackedWidget, &QStackedWidget::currentChanged, this, [=](int index) { //Sayfa değiştikçe pencere başlığını güncelle
        QWidget *aktifSayfa = ui->stackedWidget->widget(index);
        if(aktifSayfa) {
            this->setWindowTitle(aktifSayfa->windowTitle());
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

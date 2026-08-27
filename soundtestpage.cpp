#include "soundtestpage.h"
#include "ui_soundtestpage.h"
#include <QDebug>
#include <QTimer>
#include <QShowEvent>
#include <cstdlib>

void SoundTestPage::soundTestUI(){
    this->setWindowTitle("Sound Test");
    ui->yesBut->setDisabled(true);
    ui->noBut->setDisabled(true);
    ui->soundStart->setDisabled(true);
    ui->radioButton->setEnabled(true);
    ui->radioButton2->setEnabled(true);
    ui->radioButton->setChecked(false);
    ui->radioButton2->setChecked(false);
    ui->soundInfo->setText(this->windowTitle());
    ui->soundTestInfo->setText("Welcome to sound test, \n please let us know \n if you hear the sound.");
    ui->exitButton->setText("Return to the main page");
    ui->noBut->setText("No, I can not hear the audio :(");
    ui->yesBut->setText("Yes, I can hear the audio :)");
    ui->soundStart->setText("Start the sound test");
    ui->soundTestStatus->setText("The audio has not been played yet.");
    ui->formatLabel->setText("The audio format you want to try: ");
    ui->radioButton->setText(".mp3");
    ui->radioButton2->setText(".wav");

}

SoundTestPage::SoundTestPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SoundTestPage)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=](){
        ui->yesBut->setDisabled(true);
        ui->noBut->setDisabled(true);
        ui->radioButton->setEnabled(true);
        ui->radioButton2->setEnabled(true);
        refreshRadioButs();
    });
    int velocity = ui->velocitySlider->value();
    ui->velocityLabel->setText("Sound Level: " + QString::number(velocity));
}

SoundTestPage::~SoundTestPage()
{
    delete ui;
}

void SoundTestPage::refreshRadioButs(){
    ui->radioButton->setAutoExclusive(false);
    ui->radioButton2->setAutoExclusive(false);
    ui->radioButton->setChecked(false);
    ui->radioButton2->setChecked(false);
    ui->radioButton->setAutoExclusive(true);
    ui->radioButton2->setAutoExclusive(true);
}

void SoundTestPage::on_exitButton_clicked()
{
    emit returnToMainPage();
    ui->yesBut->setDisabled(true);
    ui->noBut->setDisabled(true);
    ui->soundStart->setDisabled(true);
    refreshRadioButs();
}


void SoundTestPage::on_soundStart_clicked()
{
    ui->yesBut->setEnabled(true);
    ui->noBut->setEnabled(true);
    ui->soundStart->setDisabled(true);

    QString format = "";
    if(ui->radioButton->isChecked()){
        format = ".mp3";
        system("mpg123 -q ./akbil.mp3 &");
    }
    else if (ui->radioButton2->isChecked()){
        format = ".wav";
        system("aplay ./akbil.wav &");
    }
    ui->soundTestStatus->setText("The "+ format +" audio file is playing right now...");

    QTimer::singleShot(5000, this, [=]() {
        if (ui->yesBut->isEnabled() && ui->noBut->isEnabled()) {
            ui->soundTestStatus->setText("You should tell us if you heard it or not so we can help you :)");
        }
            ui->yesBut->setDisabled(true);
            ui->noBut->setDisabled(true);
            ui->radioButton->setEnabled(true);
            ui->radioButton2->setEnabled(true);
            refreshRadioButs();
            ui->soundTestStatus->setText("Feel free if you want to make another test!");
    });
}

void SoundTestPage::on_radioButton_clicked()
{
    ui->soundStart->setEnabled(true);
}


void SoundTestPage::on_radioButton2_clicked()
{
    ui->soundStart->setEnabled(true);
}


void SoundTestPage::on_yesBut_clicked()
{
    emit yesClicked();
    ui->soundTestStatus->setText("Perfect! You're hearing the sound.");
    ui->noBut->setDisabled(true);
    ui->soundStart->setDisabled(true);
    ui->radioButton->setDisabled(true);
    ui->radioButton2->setDisabled(true);
}


void SoundTestPage::on_noBut_clicked()
{
    emit noClicked();
    ui->soundTestStatus->setText("We are sorry that you can not hear the sound.");
    ui->yesBut->setDisabled(true);
    ui->soundStart->setDisabled(true);
    ui->radioButton->setDisabled(true);
    ui->radioButton2->setDisabled(true);
}


void SoundTestPage::on_velocitySlider_valueChanged(int value)
{
    ui->velocityLabel->setText("Sound Level: " + QString::number(value));
    QString command = "amixer sset PCM " + QString::number(value) + "% &";  //PCM ses kanalına müdahele | % ->standart yüzdelik | &-> arka planda çalışabilirlik
        system(command.toStdString().c_str());
}


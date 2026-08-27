#include "touchscreentest.h"
#include "ui_touchscreentest.h"
#include <QTimer>
#include <QPushButton>
#include <QList>

void TouchScreenTest::tScreenUI(){
    this->setWindowTitle("Touch Screen Test");
    ui->pageInfo->setText(this->windowTitle());
    ui->exitButton->setText("Return to the main page");
    ui->testStart->setText("Start the touch screen test");
    ui->testStart->setEnabled(true);
    ui->statusLabel->setText("");
    ui->test1->setDisabled(true); ui->test1->setText("1"); ui->test1->setStyleSheet("");
    ui->test2->setDisabled(true); ui->test2->setText("2"); ui->test2->setStyleSheet("");
    ui->test3->setDisabled(true); ui->test3->setText("3"); ui->test3->setStyleSheet("");
    ui->test4->setDisabled(true); ui->test4->setText("4"); ui->test4->setStyleSheet("");
    ui->test5->setDisabled(true); ui->test5->setText("5"); ui->test5->setStyleSheet("");
    ui->test6->setDisabled(true); ui->test6->setText("6"); ui->test6->setStyleSheet("");
    ui->test7->setDisabled(true); ui->test7->setText("7"); ui->test7->setStyleSheet("");
    ui->test8->setDisabled(true); ui->test8->setText("8"); ui->test8->setStyleSheet("");
}

TouchScreenTest::TouchScreenTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TouchScreenTest)
{
    ui->setupUi(this);

    touchButtons = {ui->test1, ui->test2, ui->test3, ui->test4,
                    ui->test5, ui->test6, ui->test7, ui->test8
                   };
    for(QPushButton* btn : touchButtons){
        btn->setDisabled(true);
        connect(btn, &QPushButton::clicked, this, &TouchScreenTest::handleTouchButtons);
    }
    connect(ui->testStart, &QPushButton::clicked, this, &TouchScreenTest::startTouchTest);
}

TouchScreenTest::~TouchScreenTest()
{
    delete ui;
}

void TouchScreenTest::on_exitButton_clicked()
{
    emit returnToMainPage();
}
void TouchScreenTest::startTouchTest(){
    currentStep = 0;
    ui->testStart->setDisabled(true);

    for (QPushButton* btn : touchButtons) {
        btn->setDisabled(true);
        btn->setStyleSheet(""); // Varsa eski renkleri temizle
    }
    touchButtons[0]->setEnabled(true);
    touchButtons[0]->setStyleSheet("background-color: red; color: white;");
    ui->statusLabel->setText("Test has started. Please tap 1. button.");
}
void TouchScreenTest::handleTouchButtons(){
    QPushButton* clickedBtn = qobject_cast<QPushButton*>(sender());

    if (!clickedBtn || clickedBtn != touchButtons[currentStep]) {
        return;
    }
    clickedBtn->setDisabled(true); //Doğru butona tıklandı. Butonu kapat ve rengini sıfırla.
    clickedBtn->setStyleSheet("");
    currentStep++;

    if (currentStep < touchButtons.size()) {
        touchButtons[currentStep]->setEnabled(true);
        touchButtons[currentStep]->setStyleSheet("background-color: red; color: white;");

        ui->statusLabel->setText(QString("Good! Now tap the %1. button.").arg(currentStep + 1));
    } else {
    ui->statusLabel->setText("The Touch Screen test has ended successfully.");

    ui->testStart->setEnabled(true);
    }
}

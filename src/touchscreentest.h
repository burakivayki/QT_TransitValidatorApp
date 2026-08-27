#ifndef TOUCHSCREENTEST_H
#define TOUCHSCREENTEST_H

#include <QWidget>
#include <QPushButton>
#include <QList>

namespace Ui {
class TouchScreenTest;
}

class TouchScreenTest : public QWidget
{
    Q_OBJECT

public:
    explicit TouchScreenTest(QWidget *parent = nullptr);
    ~TouchScreenTest();

    void tScreenUI();


signals:
    void returnToMainPage();

private slots:
    void on_exitButton_clicked();

    void startTouchTest();

    void handleTouchButtons();

private:
    Ui::TouchScreenTest *ui;

    QList<QPushButton*> touchButtons;

    int currentStep;
};

#endif // TOUCHSCREENTEST_H

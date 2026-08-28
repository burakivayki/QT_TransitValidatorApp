#ifndef SOUNDTESTPAGE_H
#define SOUNDTESTPAGE_H

#include <QWidget>
#include <QTimer>
#include <QShowEvent>

#define FILE_FORMAT_1 ".mp3"
#define FILE_FORMAT_2 ".wav"
#define COMMAND_1 "mpg123 -q ./"
#define COMMAND_2 "aplay ./"

namespace Ui {
class SoundTestPage;
}

class SoundTestPage : public QWidget
{
    Q_OBJECT

public:
    explicit SoundTestPage(QWidget *parent = nullptr);
    ~SoundTestPage();
    QTimer *timer;

    void soundTestUI();

signals:
    void returnToMainPage();

    void yesClicked();

    void noClicked();

private slots:
    void on_exitButton_clicked();

    void on_soundStart_clicked();

    void on_radioButton_clicked();

    void on_radioButton2_clicked();

    void on_yesBut_clicked();

    void on_noBut_clicked();

    void on_velocitySlider_valueChanged(int value);

private:
    Ui::SoundTestPage *ui;

    void refreshRadioButs();
};

#endif // SOUNDTESTPAGE_H

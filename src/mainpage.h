#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QWidget>

namespace Ui {
class MainPage;
}

class MainPage : public QWidget
{
    Q_OBJECT

public:
    explicit MainPage(QWidget *parent = nullptr);
    ~MainPage();

signals:
    void openQrScanner();

    void openSoundTest();

    void openTouchScreenTest();

    void openScanCardPage();

private slots:
    void on_exitAppBut_clicked();

    void on_qrWidgetBut_clicked();

    void on_soundWidgetBut_clicked();

    void on_tScreenWidgetBut_clicked();

    void on_scanCardBut_clicked();

private:
    Ui::MainPage *ui;
};

#endif // MAINPAGE_H

#ifndef SCANCARDPAGE_H
#define SCANCARDPAGE_H

#include <QWidget>
#include <QSerialPort>

#define READER_STX_1 0x02
#define READER_STX_2 0x00

namespace Ui{
class ScanCardPage;
}

class ScanCardPage : public QWidget
{
    Q_OBJECT

public:
    explicit ScanCardPage(QWidget *parent = nullptr);
    ~ScanCardPage();

    void cardPageUI();

signals:

    void returnToMainPage();

private slots:

    void on_exitButton_clicked();
    void on_getInfBut_clicked();
    void readSerialData();
    void on_ter1_clicked();
    void on_refreshBut_clicked();

private:

    Ui::ScanCardPage*ui;
    QSerialPort*serialPort;

    QByteArray buffer;

    QByteArray rec_buffer;

};

#endif//SCANCARDPAGE_H

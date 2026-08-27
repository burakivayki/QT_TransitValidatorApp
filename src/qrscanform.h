#ifndef QRSCANFORM_H
#define QRSCANFORM_H

#include <QWidget>
#include <QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui { class QrScanForm;}
QT_END_NAMESPACE

class QrScanForm : public QWidget
{
    Q_OBJECT

public:
    explicit QrScanForm(QWidget *parent = nullptr);
    ~QrScanForm();
    void qrUI();

signals:
    void returnToMainPage();

private slots:
    void readQrData(); //Seri porta veri geldiğinde tetiklenecek fonksiyon

    void on_exitButton_clicked();

private:
    Ui::QrScanForm *ui;
    QSerialPort *serialPort; // Haberleşme nesnesi
    QByteArray buffer;       // Gelen veriyi biriktireceğimiz alan
};

#endif // QRSCANFORM_H

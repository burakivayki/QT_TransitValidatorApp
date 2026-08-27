#ifndef READER_H
#define READER_H

#include <QObject>
#include <QSerialPort>
#include <QByteArray>
#include <QString>

class Reader : public QObject
{
    Q_OBJECT
public:
    explicit Reader(QSerialPort *port, QObject *parent = nullptr);

public slots:
    void readSerialData(); // Taşınan ana fonksiyon

signals:
    void sendLogToUI(const QString &message); // Arayüze mesaj gönderecek sinyal

private:
    QSerialPort *m_serialPort;
    QByteArray rec_buffer;
};

#endif // READER_H

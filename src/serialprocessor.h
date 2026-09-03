#ifndef SERIALPROCESSOR_H
#define SERIALPROCESSOR_H

#include <QObject>
#include <QByteArray>
#include <QString>

#define READER_STX_1 0x02
#define READER_STX_2 0x00
#define READER_LEN_1 0x80
#define READER_LEN_2 0x81
#define READER_LEN_3 0x82
#define READER_ETX_1 0x03

class SerialProcessor : public QObject {
    Q_OBJECT

public:
    explicit SerialProcessor(QObject *parent = nullptr);

public slots:
    void processData(const QByteArray &data);

signals:
    void messageReady(const QString &message);

private:
    QByteArray rec_buffer;

    bool findSTX();
    int findLEN(quint32 &packetLength, int &lenSize);
    void mapFields(quint32 packetLength, int lenSize, quint8 &stx, quint8 &pcb, quint8 &ins, quint8 &calcLRC, quint8 &receivedLRC, quint8 &etx, QByteArray &dataField);
    void sendMessage(quint32 packetLength, int lenSize, quint8 stx, quint8 pcb, quint8 ins, quint8 calcLRC, quint8 receivedLRC, quint8 etx, QByteArray dataField);
};
#endif // SERIALPROCESSOR_H

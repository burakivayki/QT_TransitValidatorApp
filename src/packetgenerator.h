#ifndef PACKETGENERATOR_H
#define PACKETGENERATOR_H

#include <QObject>
#include <QByteArray>
#include <QString>

class SerialProcessor : public QObject {
    Q_OBJECT
public:
    static QByteArray createPacket(quint8 ins, const &data);

};

#endif // PACKETGENERATOR_H

#ifndef PACKETGENERATOR_H
#define PACKETGENERATOR_H

#include <QByteArray>

class PacketGenerator {
public:
    static QByteArray createPacket(quint8 ins, const QByteArray &data = QByteArray());
};

#endif // PACKETGENERATOR_H

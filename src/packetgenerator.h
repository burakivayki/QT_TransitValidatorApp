#ifndef PACKETGENERATOR_H
#define PACKETGENERATOR_H

#include <QByteArray>

class PacketGenerator {
public:
    static QByteArray createPacket(quint8 stx, quint8 pcb, quint8 ins, quint8 etx, quint16 tag, quint8 len, QByteArray value);
    static QByteArray dataFieldGenerator();
};

#endif // PACKETGENERATOR_H

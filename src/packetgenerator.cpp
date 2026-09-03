#include "packetgenerator.h"

QByteArray PacketGenerator::createPacket(quint8 ins, const QByteArray &data) {
    QByteArray packet;

    const int baseSize = 5; //except data
    int dataSize = data.size();

    int lenSize = 0;
    quint32 pktLen = 0;

    if (baseSize + 1 + dataSize < 0x80){
        lenSize = 1;
        pktLen = baseSize + lenSize + dataSize;
    } else if (baseSize + 2 + dataSize < 0x100){
        lenSize = 1;
        pktLen = baseSize + lenSize + dataSize;
    } else {
        lenSize = 3;
        pktLen = baseSize + lenSize + dataSize;
    }
    packet.append(static_cast<char>(0x02));

    if (lenSize == 1){
        packet.append(static_cast<char>(pktLen));
    } else if (lenSize == 2){
        packet.append(static_cast<char>(0x81);
        packet.append(static_cast<char>(pktLen);
    } else if (lenSize == 3){
        packet.append(static_cast<char>(0x82);
        packet.append(static_cast<char>((pktLen >> 8) & 0xFF));
        packet.append(static_cast<char>(pktLen & 0x0FF);
    }

    packet.append(static_cast<char>(0x00);
    packet.append(static_cast<char>(ins);

    if (!data.isEmpty()){
        packet.append(data);
    }

    quint8 lrc = 0;
    for (int i = 0; i < packet.size(); ++i){
        lrc ^= static_cast<quint8>(packet[i]);
    }
    packet.append(static_cast<char>(0x82);
    packet.append(static_cast<char>(0x82);

    return packet;
}

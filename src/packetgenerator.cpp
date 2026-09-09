#include "packetgenerator.h"

QByteArray PacketGenerator::createPacket(quint8 stx, quint8 pcb, quint8 ins, quint8 etx, quint16 tag, quint8 len, QByteArray value){

    QByteArray packet;
    QByteArray data;
    //TAG 16 bit
    data.append(static_cast<char>((tag >> 8) & 0xFF));
    data.append(static_cast<char>(tag & 0xFF));
    //LEN 8 bitquint8
    data.append(static_cast<char>(len));
    data.append(value);

    quint8 pktlen = 6 + data.size();

    packet.append(stx);
    packet.append(pktlen);
    packet.append(pcb);
    packet.append(ins);
    packet.append(data);

    quint8 lrc = 0;
    for(int i = 0; i < packet.size(); ++i){
        lrc ^= packet.at(i);
    }
    packet.append(lrc);
    packet.append(etx);

    return packet;
}
















/*
QByteArray PacketGenerator::createPacket(quint8 ins, const QByteArray &data) {

    QByteArray packet;

    const quint8 possibleStxValues[] = {0x02, 0x00};
    quint8 stx = possibleStxValues[QRandomGenerator::global()->bounded(0,2)];

    QByteArray innerPacket;
    innerPacket.append(static_cast<char>(0x00)); //pcb is always 0x00
    innerPacket.append(ins);
    innerPacket.append(data);

    const int baseSize = 5; //stx + pcb + ins + lrc + etx
    int dataSize = data.size();
    int lenSize = 0;
    quint8 pktLen = 0;      //packet length

    if (baseSize + 1 + dataSize < 0x80){
        lenSize = 1;
        pktLen = baseSize + lenSize + dataSize;
    } else if (baseSize + 2 + dataSize < 0x100){
        lenSize = 2;
        pktLen = baseSize + lenSize + dataSize;
    } else {
        lenSize = 3;
        pktLen = baseSize + lenSize + dataSize;
    }

    packet.append(static_cast<char>(stx));

    if (lenSize == 1){
        packet.append(static_cast<char>(pktLen));
    } else if (lenSize == 2){
        packet.append(static_cast<char>(0x81));
        packet.append(static_cast<char>(pktLen));
    } else if (lenSize == 3){
        packet.append(static_cast<char>(0x82));
        packet.append(static_cast<char>((pktLen >> 8) & 0xFF));
        packet.append(static_cast<char>(pktLen & 0xFF));
    }

    packet.append(static_cast<char>(0x00));
    packet.append(static_cast<char>(ins));

    if (!data.isEmpty()){
        packet.append(data);
    }

    quint8 lrc = 0;
    for (int i = 0; i < packet.size(); ++i){
        lrc ^= static_cast<quint8>(packet[i]);
    }

    packet.append(static_cast<char>(lrc));
    packet.append(static_cast<char>(0x03));

    return packet;
}
*/

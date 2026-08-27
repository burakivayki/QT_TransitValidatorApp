#include "serialprocessor.h"

SerialProcessor::SerialProcessor(QObject *parent) : QObject(parent){
}

void SerialProcessor::processData(const QByteArray &data){
    rec_buffer.append(data);
    int startIndex = -1;

    for (int i = 0; i < rec_buffer.size(); i++){
        if ( rec_buffer[i] == (char)READER_STX_1 || rec_buffer[i] == (char)READER_STX_2){
            startIndex = i;
            break;
        }
    }
    if(startIndex == -1){
        rec_buffer.clear();
        return;
    }
    if(startIndex > 0){
        rec_buffer.remove(0, startIndex);
    }
    if(rec_buffer.size() < 2) return;

    qint8 len1 = static_cast<quint8>(rec_buffer[1]);

    qint32 packetLength = 0;
    int lenSize = 0;

    if(len1 < READER_LEN_1){
        packetLength = len1;
        lenSize = 1;
    }
    else if(len1 == READER_LEN_2){
        if(rec_buffer.size() < 3) return;
        packetLength = static_cast<quint8>(rec_buffer[2]);
        lenSize = 2;
    }
    else if(len1 == READER_LEN_3){
        if(rec_buffer.size() < 4) return;
        quint8 len2 = static_cast<quint8>(rec_buffer[2]);
        quint8 len3 = static_cast<quint8>(rec_buffer[3]);
        packetLength = (len2 << 8) | len3;
        lenSize = 3;
    }
    else {
        rec_buffer.remove(0, 1);
        return;
    }

    if(rec_buffer.size() < (int)packetLength) return;

    QByteArray packet = rec_buffer.left(packetLength);
    rec_buffer.remove(0, packetLength);

    if(packet.endsWith(READER_ETX_1)){
        quint8 stx = static_cast<quint8>(packet[0]);
        bool isValid = true;

        if(stx == READER_STX_1){
            quint8 calcLRC = 0;
                for(int i = 0; i < packetLength - 2; ++i){
                    calcLRC ^= static_cast<quint8>(packet[i]);
                }
            quint8 receivedLRC = static_cast<quint8>(packet[packetLength-2]);
            if(calcLRC != receivedLRC){
                isValid = false;
                emit messageReady("\nError: LRC discrepancy");
            }
        }
        if(isValid){
            int headerSize = 1 + lenSize + 1 + 1;
            int footerSize = 2;
            int dataSize = packetLength - headerSize - footerSize;
            quint8 ins = static_cast<quint8>(packet[1 + lenSize + 1]);
            QByteArray dataField;

            if(dataSize > 0){
                dataField = packet.mid(headerSize, dataSize);
            }
            QString output = QString("\nINS:%1\nDATA:%2")
                        .arg(QString::number(ins,16).toUpper().rightJustified(2,'0'))
                        .arg(QString(dataField.toHex(' ').toUpper()));
            emit messageReady("\n[Packet Received]\n" + output);
        }else {
            emit messageReady("\nError:ETX byte could not find");
        }
    }
}



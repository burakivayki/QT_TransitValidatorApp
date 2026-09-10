#include "serialprocessor.h"
#include <QDateTime>

SerialProcessor::SerialProcessor(QObject *parent) : QObject(parent){
}
void SerialProcessor::processData(const QByteArray &data){
    rec_buffer.append(data);

    while (!rec_buffer.isEmpty()){
        if (findSTX() == false) {
            return;
        }
        quint32 packetLength = 0;
        int lenSize = 0;

        int lenStatus = findLEN(packetLength, lenSize);
        if (lenStatus == 1) return;
        if (lenStatus == 2) continue;

        quint8 stx = 0, calcLRC = 0, receivedLRC = 0, pcb = 0, ins = 0, etx = 0;
        QByteArray dataField;

        mapFields(packetLength, lenSize, stx, pcb, ins, calcLRC, receivedLRC, etx, dataField);
        sendMessage(packetLength, lenSize, stx, pcb, ins, calcLRC, receivedLRC, etx, dataField);
        rec_buffer.remove(0, packetLength);
    }
}
bool SerialProcessor::findSTX() {
    int startIndex = -1;
    for (int i = 0; i < rec_buffer.size(); ++i){
        if (rec_buffer[i] == (char)READER_STX_1 || rec_buffer[i] == (char)READER_STX_2){
            startIndex = i;
            break;
        }
    }
    if (startIndex == -1){
        rec_buffer.clear();
        return false;
    }
    if (startIndex > 0){
        rec_buffer.remove(0, startIndex);
    }
    if (rec_buffer.size() < 2) return false;

    return true;
}

int SerialProcessor::findLEN(quint32 &packetLength, int &lenSize) {
    quint8 len1 = static_cast<quint8>(rec_buffer[1]);

    if (len1 < READER_LEN_1){
        packetLength = len1;
        lenSize = 1;
    }
    else if (len1 == READER_LEN_2){
        if (rec_buffer.size() < 3) return 1;
        packetLength = static_cast<quint8>(rec_buffer[2]);
        lenSize = 2;
    }
    else if (len1 == READER_LEN_3){
        if (rec_buffer.size() < 4) return 1;
        quint8 len2 = static_cast<quint8>(rec_buffer[2]);
        quint8 len3 = static_cast<quint8>(rec_buffer[3]);
        packetLength = (len2 << 8) | len3;
        lenSize = 3;
    }
    else {
        rec_buffer.remove(0,1);
        return 2;
    }
    if (rec_buffer.size() < (int)packetLength) return 1;

    return 0;
}

void SerialProcessor::mapFields(quint32 packetLength, int lenSize, quint8 &stx, quint8 &pcb, quint8 &ins, quint8 &calcLRC, quint8 &receivedLRC, quint8 &etx, QByteArray &dataField) {
    int pcbIndex = 1 + lenSize;
    int insIndex = pcbIndex + 1;
    int dataStartIndex = insIndex + 1;
    int lrcIndex = packetLength - 2;
    int etxIndex = packetLength - 1;

    for (int i = 0; i < (int)packetLength; ++i){
        quint8 currentByte = static_cast<quint8>(rec_buffer[i]);

        if (i < lrcIndex){
            calcLRC ^= currentByte;
        }

        if (i == 0){
            stx = currentByte;
        }else if (i == pcbIndex){
            pcb = currentByte;
        }else if (i == insIndex){
            ins = currentByte;
        }else if (i >= dataStartIndex && i < lrcIndex){
            dataField.append(currentByte);
        }else if (i == lrcIndex){
            receivedLRC = currentByte;
        }else if (i == etxIndex){
            etx = currentByte;
        }
    }
}

QString SerialProcessor::decodeDataField(const QByteArray &dataField){
    QString parsedOutput = "";
    if (dataField.size() < 3) return parsedOutput; //2 byte tag + 1 byte len

    int index = 0;

    QByteArray responseID = dataField.mid(index, 3);
    QString responseString;

    if (static_cast<quint8>(responseID[index]) == 0xFF){
        switch (responseID[index + 1]){
            case 0x01:
                responseString = "SUCCESS TEMPLATE";
            break;
            case 0x02:
                responseString = "UNSUPPORTED TEMPLATE";
            break;
            case 0x03:
                responseString = "ERROR TEMPLATE";
            break;
        }
    } else {
        messageReady("\nUncorrect Response Id\n");
        return responseID;
    }

    index += 3;

    if (index + 2 > dataField.size()) return parsedOutput; //tagın dışına çıkıyor mu diye kontrol

    quint16 tag = (static_cast<quint8>(dataField[index]) << 8) | static_cast<quint8>(dataField[index + 1]);
    index += 2;

    quint8 len = static_cast<quint8>(dataField[index]);
    index += 1;

    switch (tag){
        case 0x0DF0C:{ //getinf -> 4 UID + 4FW + UTC + 27 RFU
            if (dataField.size() - index >= 39){
                QByteArray uid = dataField.mid(index, 4); //.mid bir yeri kesip oradan alt dizi olşturur -> ilk index'ten başlar ve 4 tane byte okuo
                quint8 fwMajor = static_cast<quint8>(dataField[index + 4]); //uid'den sonra başlamak için 4 sonrası
                quint8 fwMinor = static_cast<quint8>(dataField[index + 5]);
                quint8 fwBugfix = static_cast<quint8>(dataField[index + 6]);
                quint8 fwBuildNo = static_cast<quint8>(dataField[index + 7]);
                QByteArray utcHex = dataField.mid(index + 8, 4); //indexten 8 sonrasından başlar 4 byte keser ve utc diye yeni diziye kopyalar
                                                              //aslında kesme denmez ama olsun
                quint64 epochSecs =utcHex.toHex().toLongLong(nullptr, 16);
                QDateTime dateTime = QDateTime::fromSecsSinceEpoch(epochSecs);

                quint8 responseLen = static_cast<quint8>(responseID[2]);

                parsedOutput += "\n[Parsed DATA - GET INF]\n";
                parsedOutput += "Response Check: " + responseString + "\n";

                if (static_cast<int>(responseLen) != dataField.size()){
                    messageReady("\nERROR: Response length error.\n");
                }

                parsedOutput += "UID: " + uid.toHex(' ').toUpper() + "\n";
                parsedOutput += QString("FW_VER: %1.%2.%3.%4\n").arg(fwMajor).arg(fwMinor).arg(fwBugfix).arg(fwBuildNo); //arg hexi tamsayıya da dönüştürür
                parsedOutput += "UTC HEX: " + utcHex.toHex(' ').toUpper() + "\n";
                parsedOutput += "Epoch time: " + QString::number(epochSecs) + "\n";
                parsedOutput += "Date Time: " + dateTime.toString(Qt::ISODate) + "\n";
            } else {
                parsedOutput += "\n[Error] Hatalı GET INF uzunluğu.\n";
            }
            break;
        }
    default:
        parsedOutput += QString("\n[Unknown TAG] %1\n").arg(tag, 4, 16, QChar('0')).toUpper();
        break;
    }
    return parsedOutput;
}


void SerialProcessor::sendMessage(quint32 packetLength, int lenSize, quint8 stx, quint8 pcb, quint8 ins, quint8 calcLRC, quint8 receivedLRC, quint8 etx, QByteArray dataField) {
    bool isValid = true;

    if (stx == 0x02 && calcLRC != receivedLRC){
        isValid = false;
        emit messageReady("\nError: LRC discrepancy");
    }
    if (etx != 0x03){
        isValid = false;
        emit messageReady("\nError: ETX byte could not find");
    }

    if (isValid){
        QByteArray fullPacket = rec_buffer.left(packetLength);
        QByteArray lenBytes = rec_buffer.mid(1, lenSize);

        QString output = QString("[Packet Received]\n"
                                 "Received Packet: %1\n\n"
                                 "STX: %2\n"
                                 "LEN: %3\n"
                                 "PCB: %4\n"
                                 "INS: %5\n"
                                 "DATA: %6\n"
                                 "LRC: %7\n"
                                 "ETX: %8\n")
        .arg(QString(fullPacket.toHex(' ').toUpper()))
        .arg(QString::number(stx, 16).toUpper().rightJustified(2, '0'))
        .arg(QString(lenBytes.toHex(' ').toUpper()))
        .arg(QString::number(pcb, 16).toUpper().rightJustified(2, '0'))
        .arg(QString::number(ins, 16).toUpper().rightJustified(2, '0'))
        .arg(QString(dataField.toHex(' ').toUpper()))
        .arg(QString::number(receivedLRC, 16).toUpper().rightJustified(2, '0'))
        .arg(QString::number(etx, 16).toUpper().rightJustified(2, '0'));

        //DATA AYIRACI
        output += decodeDataField(dataField);

        emit messageReady(output);
    }
}

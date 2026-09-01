#include "serialprocessor.h"

SerialProcessor::SerialProcessor(QObject *parent) : QObject(parent){
}

void SerialProcessor::processData(const QByteArray &data){
    rec_buffer.append(data);

    while (!rec_buffer.isEmpty()){ //buffer dolu oldukça çalışacak
        int startIndex = -1;
        for (int i = 0; i < rec_buffer.size(); ++i){ //STX bulma
            if (rec_buffer[i] == (char)READER_STX_1 || rec_buffer[i] == (char)READER_STX_2){
                startIndex = i;
                break;
            }
        }
        if (startIndex == -1){ //STX bulunmama ihtimali
            rec_buffer.clear();
            return;
        }
        if (startIndex > 0){ //STX eğer 0'dan daha büyük bir indeksteyse başında başka bir şey gelmiştir
            rec_buffer.remove(0, startIndex); //stx öncesini siler
        }
        if (rec_buffer.size() < 2) return;

        quint8 len1 = static_cast<quint8>(rec_buffer[1]);
        quint32 packetLength = 0;
        int lenSize = 0;

        if (len1 < READER_LEN_1){   //PktLen < 0x80, LEN is 1 byte and LEN1 = PktLen
            packetLength = len1;
            lenSize = 1;
        }
        else if (len1 == READER_LEN_2){ //0x7F < PktLen < 0x100, LEN is 2 bytes, LEN1 = 0x81 LEN2 = PktLen
            if (rec_buffer.size() < 3) return; //ikinci byte gelmediys bkele
            packetLength = static_cast<quint8>(rec_buffer[2]);
            lenSize = 2;
        }
        else if (len1 == READER_LEN_3){ //0x0xFF < PktLen, LEN is 2 bytes, LEN1 = 0x82, LEN2 = MSB(PktLen), LEN3 = LSB(PktLen)
            if (rec_buffer.size() < 4) return; //üçüncü byte gelmediyse bekle
            quint8 len2 = static_cast<quint8>(rec_buffer[2]);
            quint8 len3 = static_cast<quint8>(rec_buffer[3]);
            packetLength = (len2 << 8) | len3;
            lenSize = 3;
        }
        else {
            rec_buffer.remove(0,1); //Hatalı bir LEN byte'ı ise, STX'i atıp yeni bir paket aramaya geç
            continue;
        }

        if (rec_buffer.size() < (int)packetLength) return; //paketin tamamı seri porttan henüz gelmediyse fonksiyon sonlandır, veriyi beklet

        quint8 stx = 0, calcLRC = 0, receivedLRC = 0, pcb = 0, ins = 0, etx = 0;
        QByteArray dataField;
        bool isValid = true;

        //indeks belirleme
        int pcbIndex = 1 + lenSize;         //STX(1) + LEN BOYUTU
        int insIndex = pcbIndex + 1;        //PCB'DEN HEMEN SONRA
        int dataStartIndex = insIndex + 1;  //INS'DEN HEMEN SONRA
        int lrcIndex = packetLength - 2;    //PAKETİN SONDAN İKİNCİ BAYTI
        int etxIndex = packetLength - 1;    //PAKETİN SON BAYTI

        for (int i = 0; i < (int)packetLength; ++i){
            quint8 currentByte = static_cast<quint8>(rec_buffer[i]);

            if (i < lrcIndex){ //LRC Hesap -> LRC baytına kadar tüm verilerin xor'unu alır
                calcLRC ^= currentByte;
            }
            if (i == 0){
                stx = currentByte;
            }else if (i == pcbIndex){ //idekslere gör e baytları değişkenlere atıo
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
                                     "Full Received RX: %1\n\n"
                                     "STX: %2\n"
                                     "LEN: %3\n"
                                     "PCB: %4\n"
                                     "INS: %5\n"
                                     "DATA: %6\n"
                                     "LRC: %7\n"
                                     "ETX: %8\n")
            .arg(QString(fullPacket.toHex(' ').toUpper())) //.toHEX(' ') -> hex'e çeviri | .toUpper() -> büyük karakter yapma
            .arg(QString::number(stx, 16).toUpper().rightJustified(2, '0')) //.rightJustified(2, '0') -> metni 2 genişliğe getirene kadar 0 ekler
            .arg(QString(lenBytes.toHex(' ').toUpper()))
            .arg(QString::number(pcb, 16).toUpper().rightJustified(2, '0'))//QString::number(stx, 16) -> 16'lık tabandaki metne (hex'e) çevirir
            .arg(QString::number(ins, 16).toUpper().rightJustified(2, '0'))
            .arg(QString(dataField.toHex(' ').toUpper()))
            .arg(QString::number(receivedLRC, 16).toUpper().rightJustified(2, '0'))
            .arg(QString::number(etx, 16).toUpper().rightJustified(2, '0'));

            emit messageReady(output);
        }
        rec_buffer.remove(0, packetLength); //işlenen paketi bufferdan al
    } //while döngüsü devam etçek
}



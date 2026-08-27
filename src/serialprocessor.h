#ifndef SERIALPROCESSOR_H
#define SERIALPROCESSOR_H

#include <QObject>
#include <QByteArray>
#include <QString>

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
};

#endif // SERIALPROCESSOR_H

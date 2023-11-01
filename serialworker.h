// serialworker.h
#ifndef SERIALWORKER_H
#define SERIALWORKER_H

#include <QObject>
#include <QSerialPort>

class SerialWorker : public QObject {
    Q_OBJECT

public:
    explicit SerialWorker(QObject *parent = nullptr);
    ~SerialWorker();

public slots:
    void doCloseSerialPort();
    void sendUint8Data(const uint8_t *data, size_t size);

    //void doWriteData(const uint8_t* data, size_t size);
    void doOpenSerialPort(const QString &portName, int baudRate, QSerialPort::Parity parity,
                              QSerialPort::DataBits dataBits, QSerialPort::StopBits stopBits,
                              QSerialPort::FlowControl flowControl);

signals:
    void resultReady(bool success);

private:
    QSerialPort *m_serialPort;
};

#endif // SERIALWORKER_H

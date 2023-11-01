// serialworker.cpp
#include "serialworker.h"
#include <QDebug>
SerialWorker::SerialWorker(QObject *parent) :
    QObject(parent),
    m_serialPort(new QSerialPort(this))
{
}

SerialWorker::~SerialWorker()
{

    if (m_serialPort && m_serialPort->isOpen()) {
        m_serialPort->close();
    }
    delete m_serialPort;
}

// SerialWorker::doOpenSerialPort 实现
void SerialWorker::doOpenSerialPort(const QString &portName, int baudRate, QSerialPort::Parity parity,
                                    QSerialPort::DataBits dataBits, QSerialPort::StopBits stopBits,
                                    QSerialPort::FlowControl flowControl) {
    qDebug() << "尝试打开串口：" << portName;
    m_serialPort->setPortName(portName);
    m_serialPort->setBaudRate(baudRate);
    m_serialPort->setParity(parity);
    m_serialPort->setDataBits(dataBits);
    m_serialPort->setStopBits(stopBits);
    m_serialPort->setFlowControl(flowControl);

    if (m_serialPort->open(QIODevice::ReadWrite)) {
        qDebug() << "串口成功打开。";
        emit resultReady(true);
    } else {
        qDebug() << "无法打开串口：" << m_serialPort->errorString();
        emit resultReady(false);
    }
}


void SerialWorker::sendUint8Data(const uint8_t *data, size_t size)
{
    if (m_serialPort->isOpen()) {
        // 将 uint8_t 数组转换为 QByteArray
        QByteArray byteArray(reinterpret_cast<const char*>(data), static_cast<int>(size));
        // 发送数据给串口
        m_serialPort->write(byteArray);
    }
}

void SerialWorker::doCloseSerialPort() {
    qDebug() << "close port" << endl;
    if (m_serialPort && m_serialPort->isOpen()) {
        m_serialPort->close();
    }
}

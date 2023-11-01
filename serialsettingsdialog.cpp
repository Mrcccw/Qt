// serialsettingsdialog.cpp
#include "serialsettingsdialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QDebug>

SerialSettingsDialog::SerialSettingsDialog(QWidget *parent)
    : QDialog(parent), isOpen(false)
{
    setWindowTitle("串口设置");
    setFixedSize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout(this);

    portLabel = new QLabel("串口号：", this);
    portComboBox = new QComboBox(this);

    // 添加可用的串口列表
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        portComboBox->addItem(info.portName());
    }

    baudRateLabel = new QLabel("波特率：", this);
    baudRateComboBox = new QComboBox(this);
    baudRateComboBox->addItem("9600");
    baudRateComboBox->addItem("115200");
    baudRateComboBox->addItem("230400");
    baudRateComboBox->addItem("460800");

    parityLabel = new QLabel("校验位：", this);
    parityComboBox = new QComboBox(this);
    parityComboBox->addItem("None", QSerialPort::NoParity);
    parityComboBox->addItem("Even", QSerialPort::EvenParity);
    parityComboBox->addItem("Odd", QSerialPort::OddParity);

    dataBitsLabel = new QLabel("数据位：", this);
    dataBitsComboBox = new QComboBox(this);
    dataBitsComboBox->addItem("5", QSerialPort::Data5);
    dataBitsComboBox->addItem("6", QSerialPort::Data6);
    dataBitsComboBox->addItem("7", QSerialPort::Data7);
    dataBitsComboBox->addItem("8", QSerialPort::Data8);
    // ... 默认设置为8位 ...

    stopBitsLabel = new QLabel("停止位：", this);
    stopBitsComboBox = new QComboBox(this);
    stopBitsComboBox->addItem("1", QSerialPort::OneStop);
    stopBitsComboBox->addItem("1.5", QSerialPort::OneAndHalfStop);
    stopBitsComboBox->addItem("2", QSerialPort::TwoStop);

    flowControlLabel = new QLabel("流控制：", this);
    flowControlComboBox = new QComboBox(this);
    flowControlComboBox->addItem("None", QSerialPort::NoFlowControl);
    flowControlComboBox->addItem("RTS/CTS", QSerialPort::HardwareControl);
    flowControlComboBox->addItem("XON/XOFF", QSerialPort::SoftwareControl);
        // ... 默认无流控制 ...

    connectButton = new QPushButton("打开", this); // 初始文本为"打开"
    exitButton = new QPushButton("退出", this);

    layout->addWidget(portLabel);
    layout->addWidget(portComboBox);
    layout->addWidget(baudRateLabel);
    layout->addWidget(baudRateComboBox);
    layout->addWidget(parityLabel);
    layout->addWidget(parityComboBox);
    layout->addWidget(dataBitsLabel);
    layout->addWidget(dataBitsComboBox);
    layout->addWidget(stopBitsLabel);
    layout->addWidget(stopBitsComboBox);
    layout->addWidget(flowControlLabel);
    layout->addWidget(flowControlComboBox);

    // 设置默认的选项
    baudRateComboBox->setCurrentText("460800");
    parityComboBox->setCurrentIndex(parityComboBox->findData(QSerialPort::NoParity));
    dataBitsComboBox->setCurrentIndex(dataBitsComboBox->findData(QSerialPort::Data8));
    stopBitsComboBox->setCurrentIndex(stopBitsComboBox->findData(QSerialPort::OneStop));
    flowControlComboBox->setCurrentIndex(flowControlComboBox->findData(QSerialPort::NoFlowControl));


    layout->addWidget(connectButton);
    layout->addWidget(exitButton);
    connectionTimer = new QTimer(this); // 初始化定时器
    connectionTimer->setSingleShot(true); // 设置定时器只触发一次
    connect(connectButton, &QPushButton::clicked, this, &SerialSettingsDialog::onConnectButtonClicked);
    connect(exitButton, &QPushButton::clicked, this, &SerialSettingsDialog::reject);
    connect(connectionTimer, &QTimer::timeout, this, &SerialSettingsDialog::handleTimeout);
    // 不再创建新的 QSerialPort 对象，而是使用传递进来的引用

    // 将串口操作移至工作线程
    serialWorker = new SerialWorker(); // 创建工作类对象
    serialWorker->moveToThread(&serialThread); // 移动到线程
    connect(&serialThread, &QThread::finished, serialWorker, &QObject::deleteLater); // 线程结束时删除工作类对象
    connect(this, &SerialSettingsDialog::openSerialPort, serialWorker, &SerialWorker::doOpenSerialPort); // 连接请求信号到工作类槽
    connect(serialWorker, &SerialWorker::resultReady, this, &SerialSettingsDialog::onSerialPortOpened); // 连接工作类结果信号到当前类槽

    serialThread.start(); // 启动线程
    connect(this, &SerialSettingsDialog::closeSerialPort, serialWorker, &SerialWorker::doCloseSerialPort);

}

SerialSettingsDialog::~SerialSettingsDialog()
{
    qDebug() << "dialog xigou" << endl;
    serialThread.quit(); // 请求线程退出
    serialThread.wait(); // 等待线程结束
}

QString SerialSettingsDialog::selectedPortName() const
{
    return portComboBox->currentText();
}

int SerialSettingsDialog::selectedBaudRate() const
{
    return baudRateComboBox->currentText().toInt();
}

void SerialSettingsDialog::onConnectButtonClicked()
{
    if (!isOpen) {
           // 启动定时器
           //connectionAttempted = true; // 尝试连接
           connectionTimer->start(1000);
           // 读取所有串口设置
           QString portName = selectedPortName();
           int baudRate = selectedBaudRate();
           QSerialPort::Parity parity = static_cast<QSerialPort::Parity>(
           parityComboBox->currentData().toInt());
           QSerialPort::DataBits dataBits = static_cast<QSerialPort::DataBits>(
           dataBitsComboBox->currentData().toInt());
           QSerialPort::StopBits stopBits = static_cast<QSerialPort::StopBits>(
           stopBitsComboBox->currentData().toInt());
           QSerialPort::FlowControl flowControl = static_cast<QSerialPort::FlowControl>(
           flowControlComboBox->currentData().toInt());
           // 发出信号请求打开串口
           emit openSerialPort(portName, baudRate, parity, dataBits, stopBits, flowControl);
               } else {
            emit closeSerialPort(); // 发送信号关闭串口
            //serialPort->close();
            //connectionAttempted = false; // 重置标志
            connectButton->setText("打开"); // 更新按钮文本
            isOpen = false; // 更新串口状态
            QMessageBox::information(this, "信息", "串口已关闭");
            emit serialPortOpenStateChanged(false); // 发出串口关闭的信号

    }
}

void SerialSettingsDialog::onSerialPortOpened(bool success) {
    connectionTimer->stop(); // 停止定时器
    if (success) {
        connectButton->setText("关闭");
        isOpen = true;
        QMessageBox::information(this, "信息", "串口已打开");
        emit serialPortOpenStateChanged(true); // 发出信号
        //accept();
    } else {
        QMessageBox::critical(this, "错误", "无法打开串口");
        emit serialPortOpenStateChanged(false); // 发出信号
        //connectionAttempted = false; // 重置标志
    }
}


void SerialSettingsDialog::handleTimeout()
{
    if (!isOpen) {
        // 如果定时器超时并且串口未打开，则关闭串口并报告失败
        emit closeSerialPort(); // 发送信号关闭串口
        QMessageBox::critical(this, "错误", "连接超时，无法打开串口");
        emit serialPortOpenStateChanged(false); // 通知主窗口串口未打开
       // connectionAttempted = false; // 重置标志，避免重复错误消息
    }
}

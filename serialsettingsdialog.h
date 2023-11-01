// serialsettingsdialog.h
#ifndef SERIALSETTINGSDIALOG_H
#define SERIALSETTINGSDIALOG_H

#include <QDialog>
#include <QSerialPort>
#include <QTimer>
#include <QThread>
#include "serialworker.h"
class QLabel;
class QComboBox;
class QPushButton;

class SerialSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    SerialSettingsDialog(QWidget *parent = nullptr);
    ~SerialSettingsDialog();
    //SerialSettingsDialog(QSerialPort *serialPort, QWidget *parent = nullptr); // 添加构造函数参数

    QString selectedPortName() const;
    int selectedBaudRate() const;
    // 添加其他串口设置的获取函数

private slots:
    void onConnectButtonClicked();
    void handleTimeout(); // 添加处理超时的槽函数
    void onSerialPortOpened(bool success); // 在这里声明槽函数
signals:
    void openSerialPort(const QString &portName, int baudRate, QSerialPort::Parity parity,
                           QSerialPort::DataBits dataBits, QSerialPort::StopBits stopBits,
                           QSerialPort::FlowControl flowControl);
    void serialPortOpenStateChanged(bool isOpen); // 新信号
    void closeSerialPort();
private:
    QLabel *portLabel;
    QLabel *baudRateLabel;
    QLabel *parityLabel;
    QLabel *dataBitsLabel;
    QLabel *stopBitsLabel;
    QLabel *flowControlLabel;
    // 添加其他串口设置的标签
    QComboBox *portComboBox;
    QComboBox *baudRateComboBox;
    QComboBox *parityComboBox;
    QComboBox *dataBitsComboBox;
    QComboBox *stopBitsComboBox;
    QComboBox *flowControlComboBox;
    // 添加其他串口设置的下拉框
    QPushButton *connectButton;
    QPushButton *exitButton;
    //QSerialPort *serialPort; // 新增串口成员变量
    bool isOpen; // 新增串口状态标志
    QTimer *connectionTimer; // 添加定时器成员
    //bool connectionAttempted = false;  // 新增标志
    QThread serialThread; // 新增一个线程成员
    SerialWorker *serialWorker; // 新增一个工作类成员
};

#endif // SERIALSETTINGSDIALOG_H

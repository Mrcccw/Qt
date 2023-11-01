#include "mainwindow.h"
#include "serialsettingsdialog.h"
#include "testdialog.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QSerialPort>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),settingsDialog(nullptr)
{
    // 创建主窗口中心的 Widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 设置窗口的大小
    resize(800, 600);

    // 创建垂直布局
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(40);  // 设置按钮之间的垂直间距为 20 像素
    // 创建四个按钮
    openSerialButton = new QPushButton("打开串口");
    upgradeButton = new QPushButton("程序升级");
    testButton = new QPushButton("进入测试");
    controlButton = new QPushButton("发射控制");

    // 设置这三个按钮为不可用状态
    upgradeButton->setDisabled(true);
    testButton->setDisabled(true);
    controlButton->setDisabled(true);

    // 使用黄金比例计算按钮的大小
    int buttonWidth = 360;
    int buttonHeight = 50;

    // 设置按钮的大小
    openSerialButton->setFixedSize(buttonWidth, buttonHeight);
    upgradeButton->setFixedSize(buttonWidth, buttonHeight);
    testButton->setFixedSize(buttonWidth, buttonHeight);
    controlButton->setFixedSize(buttonWidth, buttonHeight);

    // 将按钮添加到布局中
    layout->addWidget(openSerialButton);
    layout->addWidget(upgradeButton);
    layout->addWidget(testButton);
    layout->addWidget(controlButton);

    // 创建串口对象
   // serialPort = new QSerialPort(this);

    // 连接按钮的点击事件到相应的槽函数
    connect(openSerialButton, &QPushButton::clicked, this, &MainWindow::on_openSerialButton_clicked);
    connect(testButton, &QPushButton::clicked, this, &MainWindow::on_testButton_clicked);

    // 添加其他按钮的槽函数（程序升级、进入测试、发射控制的功能）
}

MainWindow::~MainWindow()
{
    //delete serialPort;
}

void MainWindow::on_openSerialButton_clicked()
{
    if (!settingsDialog) {
        settingsDialog = new SerialSettingsDialog(this);
        connect(settingsDialog, &SerialSettingsDialog::finished, this, &MainWindow::onSettingsDialogClosed);
        connect(settingsDialog, &SerialSettingsDialog::serialPortOpenStateChanged,this, &MainWindow::handleSerialPortOpenStateChanged);
        upgradeButton->setDisabled(true);
        testButton->setDisabled(true);
        controlButton->setDisabled(true);
    }

    settingsDialog->setModal(true); // 设置对话框为模态
    settingsDialog->exec(); // 显示对话框并进入模态循环
    //settingsDialog->show(); // 显示对话框并进入模态循环
    // 禁用其他三个按钮

    qDebug() << "Open Serial Button Clicked"; // 添加调试语句
}

void MainWindow::handleSerialPortOpenStateChanged(bool isOpen) {
    // 根据串口是否打开来启用或禁用按钮
    upgradeButton->setEnabled(isOpen);
    testButton->setEnabled(isOpen);
    controlButton->setEnabled(isOpen);
}

void MainWindow::onSettingsDialogClosed(int result)
{
    if (result == QDialog::Accepted) {
        upgradeButton->setEnabled(true);
        testButton->setEnabled(true);
        controlButton->setEnabled(true);
         qDebug() << "Buttons Enabled"; // 添加调试语句
        // 串口已连接，执行相应操作
        // 例如，更新界面上的串口状态标签
    } else {
        // 串口未连接，执行相应操作
        // 例如，禁用其他按钮或显示错误消息
        qDebug() << "Buttons Disabled"; // 添加调试语句
    }
}

void MainWindow::on_upgradeButton_clicked()
{
    // 在这里实现程序升级按钮的点击事件的功能代码
}

void MainWindow::on_testButton_clicked()
{
    if (!testDialog) {
        testDialog = new TestDialog(this);
    }

    testDialog->exec(); // 显示测试界面
}

void MainWindow::on_controlButton_clicked()
{
    // 在这里实现发射控制按钮的点击事件的功能代码
}

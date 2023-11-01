// testdialog.cpp
#include "testdialog.h"
#include <QPushButton>
#include <QVBoxLayout>

TestDialog::TestDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("测试界面");
    setFixedSize(800, 600);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(40);  // 设置按钮之间的垂直间距为 20 像素
    servoTestButton = new QPushButton("舵机测试", this);
    gyroTestButton = new QPushButton("惯组测试", this);
    recorderTestButton = new QPushButton("记录仪测试", this);
    ignitionTestButton = new QPushButton("引信测试", this);
    guidanceTestButton = new QPushButton("导引头测试", this);

    // 设置按钮的大小
    int buttonWidth = 360;
    int buttonHeight = 50;

    servoTestButton->setFixedSize(buttonWidth, buttonHeight);
    gyroTestButton->setFixedSize(buttonWidth, buttonHeight);
    recorderTestButton->setFixedSize(buttonWidth, buttonHeight);
    ignitionTestButton->setFixedSize(buttonWidth, buttonHeight);
    guidanceTestButton->setFixedSize(buttonWidth, buttonHeight);

    // 将按钮添加到垂直布局中
    layout->addWidget(servoTestButton);
    layout->addWidget(gyroTestButton);
    layout->addWidget(recorderTestButton);
    layout->addWidget(ignitionTestButton);
    layout->addWidget(guidanceTestButton);

    connect(this, &TestDialog::sendUint8DataRequest, serialWorker, &SerialWorker::sendUint8Data); // 连接请求信号到工作类槽
    connect(servoTestButton, &QPushButton::clicked, this, &TestDialog::onServoTestButtonClicked);
    connect(gyroTestButton, &QPushButton::clicked, this, &TestDialog::onGyroTestButtonClicked);
    connect(recorderTestButton, &QPushButton::clicked, this, &TestDialog::onRecorderTestButtonClicked);
    connect(ignitionTestButton, &QPushButton::clicked, this, &TestDialog::onIgnitionTestButtonClicked);
    connect(guidanceTestButton, &QPushButton::clicked, this, &TestDialog::onGuidanceTestButtonClicked);
}

TestDialog::~TestDialog()
{
}


void TestDialog::onServoTestButtonClicked()
{
    // 在这里实现舵机测试的功能代码
    // 发送数据请求信号
    // 构造一个示例的 uint8_t 数组数据
        uint8_t myData[] = {0x01, 0x02, 0x03};
        size_t dataSize = sizeof(myData);
    emit sendUint8DataRequest(myData, dataSize);
}

void TestDialog::onGyroTestButtonClicked()
{
    // 在这里实现惯组测试的功能代码
}

void TestDialog::onRecorderTestButtonClicked()
{
    // 在这里实现记录仪测试的功能代码
}

void TestDialog::onIgnitionTestButtonClicked()
{
    // 在这里实现引信测试的功能代码
}

void TestDialog::onGuidanceTestButtonClicked()
{
    // 在这里实现导引头测试的功能代码
}

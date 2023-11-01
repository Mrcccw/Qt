#include "TestWidget.h"
#include <QVBoxLayout>
#include <QMessageBox>

TestWidget::TestWidget(QWidget *parent) : QWidget(parent)
{
    setWindowTitle("测试模式");
    setFixedSize(400, 300);
    QVBoxLayout *layout = new QVBoxLayout(this);

    servoTestButton = new QPushButton("舵机测试", this);
    inertiaTestButton = new QPushButton("惯组测试", this);
    recorderTestButton = new QPushButton("记录仪测试", this);
    fuseTestButton = new QPushButton("引信测试", this);
    guidanceTestButton = new QPushButton("导引头测试", this);

    layout->addWidget(servoTestButton);
    layout->addWidget(inertiaTestButton);
    layout->addWidget(recorderTestButton);
    layout->addWidget(fuseTestButton);
    layout->addWidget(guidanceTestButton);

    connect(servoTestButton, &QPushButton::clicked, this, &TestWidget::on_servoTestButton_clicked);
    connect(inertiaTestButton, &QPushButton::clicked, this, &TestWidget::on_inertiaTestButton_clicked);
    connect(recorderTestButton, &QPushButton::clicked, this, &TestWidget::on_recorderTestButton_clicked);
    connect(fuseTestButton, &QPushButton::clicked, this, &TestWidget::on_fuseTestButton_clicked);
    connect(guidanceTestButton, &QPushButton::clicked, this, &TestWidget::on_guidanceTestButton_clicked);
}

TestWidget::~TestWidget()
{
}

void TestWidget::on_servoTestButton_clicked()
{
    QMessageBox::information(this, "舵机测试", "舵机测试正在运行...");
}

void TestWidget::on_inertiaTestButton_clicked()
{
    QMessageBox::information(this, "惯组测试", "惯组测试正在运行...");
}

void TestWidget::on_recorderTestButton_clicked()
{
    QMessageBox::information(this, "记录仪测试", "记录仪测试正在运行...");
}

void TestWidget::on_fuseTestButton_clicked()
{
    QMessageBox::information(this, "引信测试", "引信测试正在运行...");
}

void TestWidget::on_guidanceTestButton_clicked()
{
    QMessageBox::information(this, "导引头测试", "导引头测试正在运行...");
}

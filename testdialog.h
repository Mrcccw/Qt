// testdialog.h
#ifndef TESTDIALOG_H
#define TESTDIALOG_H

#include <QDialog>


class TestDialog : public QDialog
{
    Q_OBJECT

public:
    TestDialog(QWidget *parent = nullptr);
    ~TestDialog();

private slots:
    void onServoTestButtonClicked();
    void onGyroTestButtonClicked();
    void onRecorderTestButtonClicked();
    void onIgnitionTestButtonClicked();
    void onGuidanceTestButtonClicked();
signals:
    void sendUint8DataRequest(const uint8_t *data, size_t size);
private:
    QPushButton *servoTestButton;
    QPushButton *gyroTestButton;
    QPushButton *recorderTestButton;
    QPushButton *ignitionTestButton;
    QPushButton *guidanceTestButton;
};

#endif // TESTDIALOG_H

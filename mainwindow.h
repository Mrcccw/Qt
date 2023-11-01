#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serialsettingsdialog.h"
#include "testdialog.h"

class QPushButton;
class QSerialPort;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_openSerialButton_clicked();
    void on_upgradeButton_clicked();
    void on_testButton_clicked();
    void on_controlButton_clicked();
    void onSettingsDialogClosed(int result);
public slots:
    void handleSerialPortOpenStateChanged(bool isOpen);
private:
    QPushButton *openSerialButton;
    QPushButton *upgradeButton;
    QPushButton *testButton;
    QPushButton *controlButton;
    QSerialPort *serialPort;
    SerialSettingsDialog *settingsDialog;
    TestDialog *testDialog;
};

#endif // MAINWINDOW_H

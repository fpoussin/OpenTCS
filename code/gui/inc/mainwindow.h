#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "bootloader.h"
#include "tcscom.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    

private slots:
    void connect(void);
    void disconnect(void);
    void update(void);
    void importConfig(void);
    void exportConfig(void);

    void getConfig(void);
    void getData(void);
    void applyConfig(void);

private:
    Ui::MainWindow *ui;
    ftdi ftdi_device;
    bootloader bl;
    tcscom tcs;
    bool connected;

    settings_t settings;
    status_t status;
    sensors_t sensors;
    light_settings_t light_settings;
};

#endif // MAINWINDOW_H

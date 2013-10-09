#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    bl(&ftdi_device, this),
    tcs(&ftdi_device, this)
{
    ui->setupUi(this);

    QObject::connect(ui->b_connect, SIGNAL(clicked()), this, SLOT(connect()));
    QObject::connect(ui->b_disconnect, SIGNAL(clicked()), this, SLOT(disconnect()));
    QObject::connect(ui->b_update, SIGNAL(clicked()), this, SLOT(update()));
    QObject::connect(ui->b_import, SIGNAL(clicked()), this, SLOT(importConfig()));
    QObject::connect(ui->b_export, SIGNAL(clicked()), this, SLOT(exportConfig()));
    QObject::connect(ui->b_get, SIGNAL(clicked()), this, SLOT(getConfig()));
    QObject::connect(ui->b_set, SIGNAL(clicked()), this, SLOT(applyConfig()));

    ui->statusBar->showMessage("Welcome");

    this->connected = false;
}

MainWindow::~MainWindow()
{
    ftdi_device.disconnect();

    delete ui;
}

void MainWindow::connect()
{
    ui->statusBar->showMessage("Connecting...");

    if (ftdi_device.connect() == 0)
    {
        ui->statusBar->showMessage("Connected");
        this->connected = true;

        ui->b_connect->setEnabled(false);
        ui->b_disconnect->setEnabled(true);
        ui->b_set->setEnabled(true);
        ui->b_get->setEnabled(true);
        ui->b_update->setEnabled(true);

    }
    else ui->statusBar->showMessage("Connection failed");

}

void MainWindow::disconnect()
{
    tcs.disconnect();

    ui->b_connect->setEnabled(true);
    ui->b_disconnect->setEnabled(false);
    ui->b_set->setEnabled(false);
    ui->b_get->setEnabled(false);
    ui->b_update->setEnabled(false);

    ui->statusBar->showMessage("Disconnected");

    this->connected = false;
}

void MainWindow::update()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open firmware file"), "", tr("BIN Files (*.bin)"));

    QFile file(filename);
}

void MainWindow::importConfig()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open config file"), "", tr("XML Files (*.xml)"));

    QFile file(filename);
}

void MainWindow::exportConfig()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save config file"), "", tr("XML Files (*.xml)"));

    QFile file(filename);
}

void MainWindow::getConfig()
{
    tcs.getSettings(&this->settings);

    ui->cb_shifter->setChecked((settings.data.functions & SETTINGS_FUNCTION_SHIFTER));
    ui->cb_shiftlight->setChecked((settings.data.functions & SETTINGS_FUNCTION_LED));
    ui->cb_tc->setChecked((settings.data.functions & SETTINGS_FUNCTION_TC));

    ui->sb_minrpm->setValue(settings.data.min_rpm);
//    ui->sb_tcsens->setValue(settings.data.tc_base_gain);
    ui->sb_threshold->setValue(settings.data.sensor_threshold);
}

void MainWindow::getData()
{
    tcs.getDiag(&this->sensors);
    tcs.getInfo(&this->status);

    ui->le_rpm->setText(QString::number(sensors.rpm));
    ui->le_fspd->setText(QString::number(sensors.speed));
    ui->le_shift_sensor->setText(QString::number(sensors.strain_gauge));
    ui->le_tcswitch->setText(QString::number(sensors.tc_switch));
    ui->le_vbat->setText(QString::number(sensors.vbat));
}

void MainWindow::applyConfig()
{
    tcs.setSettings(&this->settings);
}

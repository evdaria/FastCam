#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Settings)
{
    ui->setupUi(this);
    ui->le_mac_sours->setInputMask("HH:HH:HH:HH:HH:HH");
    ui->le_mac_dest->setInputMask("HH:HH:HH:HH:HH:HH");
    ui->le_ip_sours->setInputMask("000.000.000.000");
    ui->le_ip_dest->setInputMask("000.000.000.000");

    loadSettings();
}

Settings::~Settings()
{
    saveSettings();
    delete ui;
}

void Settings::saveSettings() {
    QSettings settings("FastCam", "test");

    settings.setValue("macSours", ui->le_mac_sours->text());
    settings.setValue("macDest", ui->le_mac_dest->text());
    settings.setValue("ipSours", ui->le_ip_sours->text());
    settings.setValue("ipDest", ui->le_ip_dest->text());
}

void Settings::loadSettings() {
    QSettings settings("FastCam", "test");

    QString macSour = settings.value("macSours", "").toString();
    QString macDest = settings.value("macDest", "").toString();
    QString ipSour = settings.value("ipSours", "").toString();
    QString ipDest = settings.value("ipDest", "").toString();

    ui->le_mac_sours->setText(macSour);
    ui->le_mac_dest->setText(macDest);
    ui->le_ip_sours->setText(ipSour);
    ui->le_ip_dest->setText(ipDest);
}

void Settings::on_pb_save_clicked()
{
    saveSettings();
    close();
}

void Settings::on_pb_cancel_clicked()
{
    loadSettings();
    close();
}


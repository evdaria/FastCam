#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QSettings>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

private slots:
    void on_pb_save_clicked();
    void on_pb_cancel_clicked();

private:
    Ui::Settings *ui;

    void loadSettings();
    void saveSettings();
};

#endif // SETTINGS_H

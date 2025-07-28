#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QComboBox>
#include <list>
#include <memory>
#include <QLineEdit>
#include <QWidget>
#include <QFileDialog>

#include "userinputdata.h"
#include "uicontroller.h"
#include "packetprocessor.h"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void displayImage(const QImage image, uint16_t frameNumber);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setController(unique_ptr<UIController> controller);
    void setPacketProcessor(shared_ptr<PacketProcessor> packetProcessor);

private slots:
    void on_pb_start_clicked();
    void on_pb_stop_clicked();

    void on_pb_init_clicked();
    void on_pb_clk_clicked();
    void on_pb_lux_init_clicked();
    void on_pb_lux_clicked();

    void on_actionNetworkSettings_triggered();

    void on_pb_selectImage_clicked();

private:
    Ui::MainWindow *ui;

    unique_ptr<UIController> controller;
    shared_ptr<PacketProcessor> packetProcessor;

    string getSelectInterface(int index);
    list<string> getAllInterface();

    UserInputData data;
    void updateData(UserInputData &data);
};
#endif // MAINWINDOW_H

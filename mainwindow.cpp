#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"

void MainWindow::displayImage(const QImage image, uint16_t frameNumber)
{
    ui->image->setPixmap(QPixmap::fromImage(image));
    ui->imageInfo->setText("Номер кадра: " + QString::number(frameNumber));
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //line edits страницы Image
    ui->le_packetInter->setValidator(new QIntValidator(0, 65535, this));
    ui->le_fps->setValidator(new QIntValidator(1, 100, this));
    ui->le_timeExpa->setValidator(new QIntValidator(0, 16777215, this));
    ui->le_pauseAfterExpa->setValidator(new QIntValidator(0, 65535, this));
    ui->le_pix_gain->setValidator(new QIntValidator(0, 2, this));

    //line edits страницы Напряжение
    ui->le_VADH->setValidator(new QIntValidator(200, 1200, this));
    ui->le_VOFF->setValidator(new QIntValidator(0, 1200, this));
    ui->le_VLN->setValidator(new QIntValidator(400, 800, this));
    ui->le_VLNA->setValidator(new QIntValidator(700, 1200, this));
    ui->le_VLNC->setValidator(new QIntValidator(800, 1100, this));
    ui->le_VREF->setValidator(new QIntValidator(1000, 1500, this));
    ui->le_VPIX->setValidator(new QIntValidator(1200, 2500, this));
    ui->le_VRSTH->setValidator(new QIntValidator(2000, 3600, this));
    ui->le_VTXH->setValidator(new QIntValidator(2000, 3600, this));
    ui->le_VablTst->setValidator(new QIntValidator(0, 2500, this));
    ui->le_VMUX1->setValidator(new QIntValidator(800, 2500, this));
    ui->le_VMUX2->setValidator(new QIntValidator(800, 2500, this));

    //line edits страницы Синхронизация
    ui->le_clk0_phase->setValidator(new QIntValidator(0, 360, this));
    ui->le_clk1_phase->setValidator(new QIntValidator(0, 360, this));
    ui->le_clk2_phase->setValidator(new QIntValidator(0, 360, this));
    ui->le_clk3_phase->setValidator(new QIntValidator(0, 360, this));
    ui->le_clk0_PixelDelay->setValidator(new QIntValidator(0, 8, this));
    ui->le_clk1_PixelDelay->setValidator(new QIntValidator(0, 8, this));
    ui->le_clk2_PixelDelay->setValidator(new QIntValidator(0, 8, this));
    ui->le_clk3_PixelDelay->setValidator(new QIntValidator(0, 8, this));

    ui->hs_clk0_PixelDelay->setMinimum(0);
    ui->hs_clk0_PixelDelay->setMaximum(8);

    list<string> interfaces = getAllInterface();
    for (const auto& i : interfaces) {
        ui->comboBox->addItem(QString::fromStdString(i));
    }

    // Подключение slider и lineEdit

    // Синхронизация фаза

    // clk0
    connect(ui->hs_clk0_phase, &QSlider::valueChanged, this, [=](int value) {
        ui->le_clk0_phase->blockSignals(true);
        ui->le_clk0_phase->setText(QString::number(value));
        ui->le_clk0_phase->blockSignals(false);
    });
    connect(ui->le_clk0_phase, &QLineEdit::editingFinished, this, [=] {
        bool ok;
        int val = ui->le_clk0_phase->text().toInt(&ok);

        if (ok && val >= ui->hs_clk0_phase->minimum() && val <= ui->hs_clk0_phase->maximum()) {
            ui->hs_clk0_phase->blockSignals(true);
            ui->hs_clk0_phase->setValue(val);
            ui->hs_clk0_phase->blockSignals(false);
        } else {
            ui->le_clk0_phase->setText(QString::number(ui->hs_clk0_phase->value()));
        }
    });

    // clk1
    connect(ui->hs_clk1_phase, &QSlider::valueChanged, this, [=](int value) {
        ui->le_clk1_phase->blockSignals(true);
        ui->le_clk1_phase->setText(QString::number(value));
        ui->le_clk1_phase->blockSignals(false);
    });
    connect(ui->le_clk1_phase, &QLineEdit::editingFinished, this, [=] {
        bool ok;
        int val = ui->le_clk1_phase->text().toInt(&ok);

        if (ok && val >= ui->hs_clk1_phase->minimum() && val <= ui->hs_clk1_phase->maximum()) {
            ui->hs_clk1_phase->blockSignals(true);
            ui->hs_clk1_phase->setValue(val);
            ui->hs_clk1_phase->blockSignals(false);
        } else {
            ui->le_clk1_phase->setText(QString::number(ui->hs_clk1_phase->value()));
        }
    });

    // clk2
    connect(ui->hs_clk2_phase, &QSlider::valueChanged, this, [=](int value) {
        ui->le_clk2_phase->blockSignals(true);
        ui->le_clk2_phase->setText(QString::number(value));
        ui->le_clk2_phase->blockSignals(false);
    });
    connect(ui->le_clk2_phase, &QLineEdit::editingFinished, this, [=] {
        bool ok;
        int val = ui->le_clk2_phase->text().toInt(&ok);

        if (ok && val >= ui->hs_clk2_phase->minimum() && val <= ui->hs_clk2_phase->maximum()) {
            ui->hs_clk2_phase->blockSignals(true);
            ui->hs_clk2_phase->setValue(val);
            ui->hs_clk2_phase->blockSignals(false);
        } else {
            ui->le_clk2_phase->setText(QString::number(ui->hs_clk2_phase->value()));
        }
    });

    // clk3
    connect(ui->hs_clk3_phase, &QSlider::valueChanged, this, [=](int value) {
        ui->le_clk3_phase->blockSignals(true);
        ui->le_clk3_phase->setText(QString::number(value));
        ui->le_clk3_phase->blockSignals(false);
    });
    connect(ui->le_clk3_phase, &QLineEdit::editingFinished, this, [=] {
        bool ok;
        int val = ui->le_clk3_phase->text().toInt(&ok);

        if (ok && val >= ui->hs_clk3_phase->minimum() && val <= ui->hs_clk3_phase->maximum()) {
            ui->hs_clk3_phase->blockSignals(true);
            ui->hs_clk3_phase->setValue(val);
            ui->hs_clk3_phase->blockSignals(false);
        } else {
            ui->le_clk3_phase->setText(QString::number(ui->hs_clk3_phase->value()));
        }
    });

    // Синхронизация pix delay

    // clk0
    connect(ui->hs_clk0_PixelDelay, &QSlider::valueChanged, this, [=](int value) {
        ui->le_clk0_PixelDelay->blockSignals(true);
        ui->le_clk0_PixelDelay->setText(QString::number(value));
        ui->le_clk0_PixelDelay->blockSignals(false);
    });
    connect(ui->le_clk0_PixelDelay, &QLineEdit::editingFinished, this, [=] {
        bool ok;
        int val = ui->le_clk0_PixelDelay->text().toInt(&ok);

        if (ok && val >= ui->hs_clk0_PixelDelay->minimum() && val <= ui->hs_clk0_PixelDelay->maximum()) {
            ui->hs_clk0_PixelDelay->blockSignals(true);
            ui->hs_clk0_PixelDelay->setValue(val);
            ui->hs_clk0_PixelDelay->blockSignals(false);
        } else {
            ui->le_clk0_PixelDelay->setText(QString::number(ui->hs_clk0_PixelDelay->value()));
        }
    });

    // clk1
    connect(ui->hs_clk1_PixelDelay, &QSlider::valueChanged, this, [=](int value) {
        ui->le_clk1_PixelDelay->blockSignals(true);
        ui->le_clk1_PixelDelay->setText(QString::number(value));
        ui->le_clk1_PixelDelay->blockSignals(false);
    });
    connect(ui->le_clk1_PixelDelay, &QLineEdit::editingFinished, this, [=] {
        bool ok;
        int val = ui->le_clk1_PixelDelay->text().toInt(&ok);

        if (ok && val >= ui->hs_clk1_PixelDelay->minimum() && val <= ui->hs_clk1_PixelDelay->maximum()) {
            ui->hs_clk1_PixelDelay->blockSignals(true);
            ui->hs_clk1_PixelDelay->setValue(val);
            ui->hs_clk1_PixelDelay->blockSignals(false);
        } else {
            ui->le_clk1_PixelDelay->setText(QString::number(ui->hs_clk1_PixelDelay->value()));
        }
    });

    // clk2
    connect(ui->hs_clk2_PixelDelay, &QSlider::valueChanged, this, [=](int value) {
        ui->le_clk2_PixelDelay->blockSignals(true);
        ui->le_clk2_PixelDelay->setText(QString::number(value));
        ui->le_clk2_PixelDelay->blockSignals(false);
    });
    connect(ui->le_clk2_PixelDelay, &QLineEdit::editingFinished, this, [=] {
        bool ok;
        int val = ui->le_clk2_PixelDelay->text().toInt(&ok);

        if (ok && val >= ui->hs_clk2_PixelDelay->minimum() && val <= ui->hs_clk2_PixelDelay->maximum()) {
            ui->hs_clk2_PixelDelay->blockSignals(true);
            ui->hs_clk2_PixelDelay->setValue(val);
            ui->hs_clk2_PixelDelay->blockSignals(false);
        } else {
            ui->le_clk2_PixelDelay->setText(QString::number(ui->hs_clk2_PixelDelay->value()));
        }
    });

    // clk3
    connect(ui->hs_clk3_PixelDelay, &QSlider::valueChanged, this, [=](int value) {
        ui->le_clk3_PixelDelay->blockSignals(true);
        ui->le_clk3_PixelDelay->setText(QString::number(value));
        ui->le_clk3_PixelDelay->blockSignals(false);
    });
    connect(ui->le_clk3_PixelDelay, &QLineEdit::editingFinished, this, [=] {
        bool ok;
        int val = ui->le_clk3_PixelDelay->text().toInt(&ok);

        if (ok && val >= ui->hs_clk3_PixelDelay->minimum() && val <= ui->hs_clk3_PixelDelay->maximum()) {
            ui->hs_clk3_PixelDelay->blockSignals(true);
            ui->hs_clk3_PixelDelay->setValue(val);
            ui->hs_clk3_PixelDelay->blockSignals(false);
        } else {
            ui->le_clk3_PixelDelay->setText(QString::number(ui->hs_clk3_PixelDelay->value()));
        }
    });

    // Напряжение

    connect(ui->d_VablTst, &QDial::valueChanged,this, [=]{ // VABL TST
        int val = ui->d_VablTst->value();
        ui->le_VablTst->setText(QString::number(val));
    });
    connect(ui->le_VablTst, &QLineEdit::textChanged,this, [=]{
        int val = ui->le_VablTst->text().toDouble();
        ui->d_VablTst->setValue(val);
    });

    connect(ui->d_VREF, &QDial::valueChanged,this, [=]{ // VREF
        int val = ui->d_VREF->value();
        ui->le_VREF->setText(QString::number(val));
    });
    connect(ui->le_VREF, &QLineEdit::textChanged,this, [=]{
        int val = ui->le_VREF->text().toInt();
        ui->d_VREF->setValue(val);
    });

    connect(ui->d_VRSTH, &QDial::valueChanged,this, [=]{ // VRSTH
        int val = ui->d_VRSTH->value();
        ui->le_VRSTH->setText(QString::number(val));
    });
    connect(ui->le_VRSTH, &QLineEdit::textChanged,this, [=]{
        int val = ui->le_VRSTH->text().toInt();
        ui->d_VRSTH->setValue(val);
    });

    connect(ui->d_VADH, &QDial::valueChanged,this, [=]{ // VADH
        int val = ui->d_VADH->value();
        ui->le_VADH->setText(QString::number(val));
    });
    connect(ui->le_VADH, &QLineEdit::textChanged,this, [=]{
        int val = ui->le_VADH->text().toInt();
        ui->d_VADH->setValue(val);
    });

    connect(ui->d_VLN, &QDial::valueChanged,this, [=]{ // VLN
        int val = ui->d_VLN->value();
        ui->le_VLN->setText(QString::number(val));
    });
    connect(ui->le_VLN, &QLineEdit::textChanged,this, [=]{
        int val = ui->le_VLN->text().toInt();
        ui->d_VLN->setValue(val);
    });

    connect(ui->d_VLNA, &QDial::valueChanged,this, [=]{ // VLNA
        int val = ui->d_VLNA->value();
        ui->le_VLNA->setText(QString::number(val));
    });
    connect(ui->le_VLNA, &QLineEdit::textChanged,this, [=]{
        int val = ui->le_VLNA->text().toInt();
        ui->d_VLNA->setValue(val);
    });

    connect(ui->d_VLNC, &QDial::valueChanged,this, [=]{ // VLNC
        int val = ui->d_VLNC->value();
        ui->le_VLNC->setText(QString::number(val));
    });
    connect(ui->le_VLNC, &QLineEdit::textChanged,this, [=]{
        int val = ui->le_VLNC->text().toInt();
        ui->d_VLNC->setValue(val);
    });

    connect(ui->d_VMUX1, &QDial::valueChanged,this, [=]{ // VMUX1
        int val = ui->d_VMUX1->value();
        ui->le_VMUX1->setText(QString::number(val));
    });
    connect(ui->le_VMUX1, &QLineEdit::textChanged,this, [=]{
        int val = ui->le_VMUX1->text().toInt();
        ui->d_VMUX1->setValue(val);
    });

    connect(ui->d_VMUX2, &QDial::valueChanged,this, [=]{ // VMUX2
        int val = ui->d_VMUX2->value();
        ui->le_VMUX2->setText(QString::number(val));
    });
    connect(ui->le_VMUX2, &QLineEdit::textChanged,this, [=]{
        int val = ui->le_VMUX2->text().toInt();
        ui->d_VMUX2->setValue(val);
    });

    connect(ui->d_VOFF, &QDial::valueChanged,this, [=]{ // VOFF
        int val = ui->d_VOFF->value();
        ui->le_VOFF->setText(QString::number(val));
    });
    connect(ui->le_VOFF, &QLineEdit::textChanged,this, [=]{
        int val = ui->le_VOFF->text().toInt();
        ui->d_VOFF->setValue(val);
    });

    connect(ui->d_VPIX, &QDial::valueChanged,this, [=]{ // VPIX
        int val = ui->d_VPIX->value();
        ui->le_VPIX->setText(QString::number(val));
    });
    connect(ui->le_VPIX, &QLineEdit::textChanged,this, [=]{
        int val = ui->le_VPIX->text().toInt();
        ui->d_VPIX->setValue(val);
    });

    connect(ui->d_VTXH, &QDial::valueChanged,this, [=]{ // VTXH
        int val = ui->d_VTXH->value();
        ui->le_VTXH->setText(QString::number(val));
    });
    connect(ui->le_VTXH, &QLineEdit::textChanged,this, [=]{
        int val = ui->le_VTXH->text().toInt();
        ui->d_VTXH->setValue(val);
    });

    QImage image = QImage("not.jpg");
    ui->image->setMinimumSize(1000, 710);
    ui->image->setPixmap(QPixmap::fromImage(image));
    ui->image->show();

    packetProcessor = make_shared<PacketProcessor>();
    controller = make_unique<UIController>(packetProcessor);
    connect(packetProcessor.get(), &PacketProcessor::imageReady, this, &MainWindow::displayImage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pb_start_clicked()
{
    ui->pb_start->setEnabled(false);
    int interfaceIndex = ui->comboBox->currentIndex();
    std::string interfaceNameStr = getSelectInterface(interfaceIndex);
    const char *interfaceName = interfaceNameStr.c_str();

    updateData(data);

    controller->handleButtonClick(interfaceName, 1, data);
}

void MainWindow::on_pb_stop_clicked()
{
    int interfaceIndex = ui->comboBox->currentIndex();
    std::string interfaceNameStr = getSelectInterface(interfaceIndex);
    const char *interfaceName = interfaceNameStr.c_str();

    updateData(data);
    data.workMode = 0;

    controller->handleButtonClick(interfaceName, 1, data);

    ui->pb_start->setEnabled(true);
}

string MainWindow::getSelectInterface(int index)
{
    pcap_if_t *alldevs;
    pcap_if_t *device;
    char errbuf[PCAP_ERRBUF_SIZE];

    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        fprintf(stderr, "Failed to get a list of interfaces: %s\n", errbuf);
        QMessageBox::information(this, "Error", "YFailed to get a list of interfaces: %s\n", QMessageBox::Ok);
    }

    device = alldevs;
    for(int i = 0; i < index; i++) {
        device = device->next;
    }

    return device->name;
}

list<string> MainWindow::getAllInterface()
{
    pcap_if_t *alldevs;
    pcap_if_t *device;
    char errbuf[PCAP_ERRBUF_SIZE];

    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        fprintf(stderr, "Failed to get a list of interfaces: %s\n", errbuf);
        QMessageBox::information(this,"Error", "YFailed to get a list of interfaces: %s\n", QMessageBox::Ok);
    }

    list<string> availableInterfeces;
    device = alldevs;

    while (device != NULL) {
        string tmp;
        tmp.append(device->description).append("\n").append(device->name);
        availableInterfeces.push_back(tmp);
        device = device->next;
    }

    return availableInterfeces;
}

void MainWindow::updateData(UserInputData &data)
{
    data.workMode = ui->cb_mode->currentIndex();

    data.framePause = (1000000000/8)/(ui->le_fps->text().toULong());

    data.interframePause = ui->le_packetInter->text().toUShort();

    uint32_t tmpTimeExpa = ui->le_timeExpa->text().toULong();
    data.timeExpa[0] = (tmpTimeExpa & 0x000000FF);
    data.timeExpa[1] = (tmpTimeExpa & 0x0000FF00) >> 8;
    data.timeExpa[2] = (tmpTimeExpa & 0x00FF0000) >> 16;

    data.pauseAfterExpa = ui->le_pauseAfterExpa->text().toUShort();
    data.pixDelay = ui->le_clk0_PixelDelay->text().toUShort()
                     + (ui->le_clk1_PixelDelay->text().toUShort() << 4)
                     + (ui->le_clk2_PixelDelay->text().toUShort() << 8)
                     + (ui->le_clk3_PixelDelay->text().toUShort() << 12);
    data.DDR_INV = 0;
    data.PIX_INV = ((ui->checkBoxPIX_INV0->checkState()
                     + (ui->checkBoxPIX_INV1->checkState() << 1)
                     + (ui->checkBoxPIX_INV2->checkState() << 2)
                     + (ui->checkBoxPIX_INV3->checkState() << 3)
                     )>>1);
    data.STROKE_ORDER = 0;

    uint32_t clk0 = ui->le_clk0_phase->text().toULong() * 1000;
    data.GigE_PHASE_0[0] = (clk0 & 0x000000FF);
    data.GigE_PHASE_0[1] = (clk0 & 0x0000FF00) >> 8;
    data.GigE_PHASE_0[2] = (clk0 & 0x00FF0000) >> 16;

    uint32_t clk1 = ui->le_clk1_phase->text().toULong() * 1000;
    data.GigE_PHASE_1[0] = (clk1 & 0x000000FF);
    data.GigE_PHASE_1[1] = (clk1 & 0x0000FF00) >> 8;
    data.GigE_PHASE_1[2] = (clk1 & 0x00FF0000) >> 16;

    uint32_t clk2 = ui->le_clk2_phase->text().toULong() * 1000;
    data.GigE_PHASE_2[0] = (clk2 & 0x000000FF);
    data.GigE_PHASE_2[1] = (clk2 & 0x0000FF00) >> 8;
    data.GigE_PHASE_2[2] = (clk2 & 0x00FF0000) >> 16;

    uint32_t clk3 = ui->le_clk3_phase->text().toULong() * 1000;
    data.GigE_PHASE_3[0] = (clk3 & 0x000000FF);
    data.GigE_PHASE_3[1] = (clk3 & 0x0000FF00) >> 8;
    data.GigE_PHASE_3[2] = (clk3 & 0x00FF0000) >> 16;

    data.VABL_TST = ui->le_VablTst->text().toDouble()/1000 * (65535/3.9);
    data.VREF = ui->le_VREF->text().toDouble()/1000 * (65535/3.9);
    data.VRSTH = ui->le_VRSTH->text().toDouble()/1000 * (65535/3.9);
    data.VADH = ui->le_VADH->text().toDouble()/1000 * (65535/3.9);
    data.VLN = ui->le_VLN->text().toDouble()/1000 * (65535/3.9);
    data.VLNA = ui->le_VLNA->text().toDouble()/1000 * (65535/3.9);
    data.VLNC = ui->le_VLNC->text().toDouble()/1000 * (65535/3.9);
    data.VMUX1 = ui->le_VMUX1->text().toDouble()/1000 * (65535/3.9);
    data.VMUX2 = ui->le_VMUX2->text().toDouble()/1000 * (65535/3.9);
    data.VOFF = ui->le_VOFF->text().toDouble()/1000 * (65535/3.9);
    data.VPIX = ui->le_VPIX->text().toDouble()/1000 * (65535/3.9);
    data.VTXH = ui->le_VTXH->text().toDouble()/1000 * (65535/3.9);

    int selectedIndex1 = this->ui->comboBoxVMUX1->currentIndex();
    int VMUX1 = 0;
    if(selectedIndex1 == 0) VMUX1 = 0;
    else if(selectedIndex1 == 1) VMUX1 = 1;
    else if(selectedIndex1 == 2) VMUX1 = 2;
    else if(selectedIndex1 == 3) VMUX1 = 3;

    int selectedIndex2 = this->ui->comboBoxLVDSCurrent->currentIndex();
    int LVDSCurrent = 0;
    if(selectedIndex2 == 0) LVDSCurrent = 1;
    else if(selectedIndex2 == 1) LVDSCurrent = 0;
    else if(selectedIndex2 == 2) LVDSCurrent = 2;
    else if(selectedIndex2 == 3) LVDSCurrent = 6;

    uint32_t tmpLuxMess = (ui->checkBoxColTestEn->isChecked())
                 + (ui->checkBoxOffsetSign->isChecked() << 1)
                 + (ui->checkBoxGainX2En->isChecked() << 2)
                 + (ui->checkBoxGainBit1En->isChecked() << 3)
                 + (ui->checkBoxGainBit2En->isChecked() << 4)
                 + (ui->checkBoxUnipolarADC->isChecked() << 5)
                 + (ui->checkBoxLongLatch->isChecked() << 6)
                 + (ui->checkBoxRowCutDisB->isChecked() << 7)
                 + (ui->checkBoxShortXWindow->isChecked() << 8)
                 + (ui->checkBoxVLNCasEnB->isChecked() << 9)
                 + ( (VMUX1 & 0x3) << 10)
                 + (ui->checkBoxVMUX2->isChecked() << 12)
                 + (ui->checkBoxSHVLN->isChecked() << 13)
                 + (ui->checkBoxC1Pout->isChecked() << 14)
                 + (ui->checkBoxExtended->isChecked() << 15)
                 + (ui->checkBoxPwrenLvdsB->isChecked() << 16)
                 + (ui->checkBoxPwrenBiasB->isChecked() << 17)
                 + (ui->checkBoxMSBFirst->isChecked() << 18)
                 + ( (LVDSCurrent & 0x7) << 19)
                 + (ui->checkBoxTetmOffB->isChecked() << 22);
    data.LUX_MESS[0] = (tmpLuxMess & 0x000000FF);
    data.LUX_MESS[1] = (tmpLuxMess & 0x0000FF00) >> 8;
    data.LUX_MESS[2] = (tmpLuxMess & 0x00FF0000) >> 16;
}

void MainWindow::on_pb_init_clicked()
{
    updateData(data);

    int interfaceIndex = ui->comboBox->currentIndex();
    std::string interfaceNameStr = getSelectInterface(interfaceIndex);
    const char *interfaceName = interfaceNameStr.c_str();

    controller->handleButtonClick(interfaceName, 2, data);
}

void MainWindow::on_pb_clk_clicked()
{
    updateData(data);

    int interfaceIndex = ui->comboBox->currentIndex();
    std::string interfaceNameStr = getSelectInterface(interfaceIndex);
    const char *interfaceName = interfaceNameStr.c_str();

    controller->handleButtonClick(interfaceName, 3, data);
}

void MainWindow::on_pb_lux_init_clicked()
{
    updateData(data);

    int interfaceIndex = ui->comboBox->currentIndex();
    std::string interfaceNameStr = getSelectInterface(interfaceIndex);
    const char *interfaceName = interfaceNameStr.c_str();

    controller->handleButtonClick(interfaceName, 5, data);
}


void MainWindow::on_pb_lux_clicked()
{
    updateData(data);

    int interfaceIndex = ui->comboBox->currentIndex();
    std::string interfaceNameStr = getSelectInterface(interfaceIndex);
    const char *interfaceName = interfaceNameStr.c_str();

    controller->handleButtonClick(interfaceName, 4, data);
}

void MainWindow::on_actionNetworkSettings_triggered()
{
    Settings window;
    window.setWindowTitle("Settings");
    window.setModal(true);
    window.exec();
}


void MainWindow::on_pb_selectImage_clicked()
{
    auto full_name = QFileDialog::getOpenFileName(this, tr("Select image"), ".", tr("Images (*.png *.jpg)"));
    QFileInfo fileInfo(full_name);
    QString fileName = fileInfo.fileName();
    ui->le_selectImage->setText(fileName);
}


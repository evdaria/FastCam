#ifndef USERINPUTDATA_H
#define USERINPUTDATA_H

#include <QMainWindow>
#include <pcap.h>
#include <ws2tcpip.h>
#include <QMessageBox>
#include <QObject>
#include <QImage>
#include <QDateTime>
#include <QSettings>
#include <unistd.h>

class UserInputData
{
public:
    UserInputData();

    // COMMAND_WORK_MODE
    uint8_t workMode; // Режим работы

    // COMMAND_IMAGE_PARAM
    uint32_t framePause; // (fps)
    uint16_t interframePause;
    uint8_t timeExpa[3];
    uint16_t pauseAfterExpa;
    uint16_t pixDelay;
    uint8_t DDR_INV;
    uint8_t PIX_INV;//это что?
    uint8_t PIX_GAIN;
    uint8_t STROKE_ORDER;

    // COMMAND_WRITE_PLL
    uint8_t GigE_PHASE_0[3];
    uint8_t GigE_PHASE_1[3];
    uint8_t GigE_PHASE_2[3];
    uint8_t GigE_PHASE_3[3];

    // COMMAND_LUX_INIT
    uint16_t VABL_TST;
    uint16_t VREF;
    uint16_t VRSTH;
    uint16_t VADH;
    uint16_t VLN;
    uint16_t VLNA;
    uint16_t VLNC;
    uint16_t VMUX1;
    uint16_t VMUX2;
    uint16_t VOFF;
    uint16_t VPIX;
    uint16_t VTXH;

    // COMMAND_LUX_PARAM
    uint8_t LUX_MESS[3];
};

#endif // USERINPUTDATA_H

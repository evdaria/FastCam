#ifndef PACKETSENDER_H
#define PACKETSENDER_H

#include <QMainWindow>
#include <pcap.h>
#include <vector>
#include <string>
#include <ws2tcpip.h>
#include <QMessageBox>
#include <QObject>
#include <QImage>
#include <QDateTime>
#include <QSettings>
#include <unistd.h>
#include <Win32-Extensions.h>

#include "userinputdata.h"

#define ETHER_HDR_LEN 14
#define IP_HDR_LEN 20
#define UDP_HDR_LEN 8
#define DATA_HDR_LEN 11
#define DATA_LEN 64

using namespace std;

#pragma pack(push, 1)
// Определение структуры Ethernet
struct ethhdr {
    unsigned char h_dest[6]; // MAC адрес назначения
    unsigned char h_source[6]; // MAC адрес источника
    unsigned short h_proto; // Протокол
};

// Определение структуры IP
struct iphdr {
    unsigned char ihl:4; // Длина заголовка
    unsigned char version:4; // Версия
    unsigned char tos; // Тип сервиса
    unsigned short tot_len; // Общая длина
    unsigned short id; // Идентификатор
    unsigned short frag_off; // Смещение фрагмента
    unsigned char ttl; // Время жизни
    unsigned char protocol; // Протокол
    unsigned short check; // Контрольная сумма
    unsigned int saddr; // Адрес источника
    unsigned int daddr; // Адрес назначения
};

// Определение структуры UDP
struct udphdr {
    unsigned short source; // Исходный порт
    unsigned short dest; // Порт назначения
    unsigned short len; // Длина заголовка
    unsigned short check; // Контрольная сумма
};

struct datahdr {
    uint8_t signature[8]; // Сигнатура (1-8 байт)
    uint16_t packetLen; // Длина пакета (9-10 байт)
    uint8_t commandNumber; // Команда (11ый байт)
};
#pragma pack(pop)

#pragma pack(1)
struct COMMAND_WORK_MODE{
    uint8_t operationMode; // Режим работы
};
#pragma pack()

#pragma pack(1)
struct COMMAND_IMAGE_PARAM {
    uint32_t pauseFrame;
    uint16_t packetInter;
    uint8_t timeExpa[3];
    uint16_t pauseAfterExpa;
    uint16_t pixDelay;
    uint8_t DDR_INV;
    uint8_t PIX_INV;
    uint8_t PIX_GAIN;
    uint8_t STROKE_ORDER;
};
#pragma pack()

#pragma pack(1)
struct COMMAND_WRITE_PLL{
    uint8_t GigE_PHASE_0[3];
    uint8_t GigE_PHASE_1[3];
    uint8_t GigE_PHASE_2[3];
    uint8_t GigE_PHASE_3[3];
};
#pragma pack()

#pragma pack(1)
struct COMMAND_LUX_INIT{
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
};
#pragma pack()

#pragma pack(1)
struct COMMAND_LUX_PARAM{
    uint8_t LUX_MESS[3];
};
#pragma pack()

class PacketSender
{
public:
    PacketSender();

    void sendPacket(const char *interfaceName, int command, UserInputData &data);

private:
    QMainWindow* mainW;

    uint16_t calcChecksum(uint16_t *data, int size);
    uint16_t udpCalcChecksum(uint16_t *data, int size, struct iphdr *ip_header);
    vector<string> networkInfoFromFile();

    void createBasePacket(u_char *basePacket);

    void getWorkMode(u_char *packet, u_char *basePacket, UserInputData &data);
    void getInit(u_char *packet, u_char *basePacket, UserInputData &data);
    void getClk(u_char *packet, u_char *basePacket, UserInputData &data);
    void getLuxMess(u_char *packet, u_char *basePacket, UserInputData &data);
    void getLuxInit(u_char *packet, u_char *basePacket, UserInputData &data);
};

#endif // PACKETSENDER_H

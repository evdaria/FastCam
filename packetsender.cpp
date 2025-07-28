#include "packetsender.h"

PacketSender::PacketSender() {}

void PacketSender::sendPacket(const char *interfaceName, int command, UserInputData &data)
{
    pcap_t *fp;
    char errbuf[PCAP_ERRBUF_SIZE];
    fp = pcap_open_live(interfaceName, 1600, 1, 1000, errbuf);

    if (fp == NULL) {
        fprintf(stderr, "\nUnable to open the adapter in creater. %s is not supported by WinPcap\n", interfaceName);
        QMessageBox::information(mainW,"Error", "Unable to open the adapter in createAndSendPacket", QMessageBox::Ok);
        return;
    }

    size_t packetSize = ETHER_HDR_LEN + IP_HDR_LEN + UDP_HDR_LEN + DATA_LEN;

    pcap_send_queue *squeue = pcap_sendqueue_alloc(10 * (packetSize));
    struct pcap_pkthdr* hdr = (struct pcap_pkthdr*)malloc(sizeof(struct pcap_pkthdr));
    hdr->caplen = hdr->len = static_cast<int>(packetSize);


    u_char *basePacket = (u_char*)malloc(packetSize);
    memset(basePacket, 0, packetSize); // Обнуление пакета перед заполнением
    createBasePacket(basePacket);

    u_char *packet = (u_char*)malloc(packetSize);
    switch(command) {
    case 0:
    {
        getWorkMode(packet, basePacket, data);
        if (pcap_sendqueue_queue(squeue, hdr, packet) == -1) {
            fprintf(stderr, "Error queuing the packet %s\n", pcap_geterr(fp));
        }
        break;
    }
    case 1:
    {
        getWorkMode(packet, basePacket, data);
        if (pcap_sendqueue_queue(squeue, hdr, packet) == -1) {
            fprintf(stderr, "Error queuing the packet %s\n", pcap_geterr(fp));
        }
        break;
    }
    case 2:
    {
        getInit(packet, basePacket, data);
        if (pcap_sendqueue_queue(squeue, hdr, packet) == -1) {
            fprintf(stderr, "Error queuing the packet %s\n", pcap_geterr(fp));
        }
        break;
    }
    case 3:
    {
        getClk(packet, basePacket, data);
        if (pcap_sendqueue_queue(squeue, hdr, packet) == -1) {
            fprintf(stderr, "Error queuing the packet %s\n", pcap_geterr(fp));
        }
        break;
    }
    case 4:
    {
        getLuxMess(packet, basePacket, data);
        if (pcap_sendqueue_queue(squeue, hdr, packet) == -1) {
            fprintf(stderr, "Error queuing the packet %s\n", pcap_geterr(fp));
        }
        break;
    }
    case 5:
    {
        getLuxInit(packet, basePacket, data);
        if (pcap_sendqueue_queue(squeue, hdr, packet) == -1) {
            fprintf(stderr, "Error queuing the packet %s\n", pcap_geterr(fp));
        }
        break;
    }
    }

    if (pcap_sendqueue_transmit(fp, squeue, false) == 1) {
        fprintf(stderr, "Error sending the packet queue: %sn\n", pcap_geterr(fp));
    }

    free(basePacket);
    free(packet);
    free(hdr);
    pcap_sendqueue_destroy(squeue);

    pcap_close(fp);
}

uint16_t PacketSender::calcChecksum(uint16_t *data, int size)
{
    uint32_t sum = 0;
    for (int i = 0; i < size / 2; i++) {
        sum += data[i];
    }
    if (size % 2) {
        sum += data[size / 2] & 0xFF00;
    }
    while (sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }
    return (uint16_t)(~sum);
}

uint16_t PacketSender::udpCalcChecksum(uint16_t *data, int size, iphdr *ip_header)
{
    uint32_t sum = 0;

    // Псевдозаголовок
    sum += (ip_header->saddr >> 16) & 0xFFFF;
    sum += ip_header->saddr & 0xFFFF;
    sum += (ip_header->daddr >> 16) & 0xFFFF;
    sum += ip_header->daddr & 0xFFFF;
    sum += htons(IPPROTO_UDP); // Протокол UDP
    sum += htons(size); // Длина UDP

    for (int i = 0; i < size / 2; i++) {
        sum += data[i];
    }
    if (size % 2) {
        sum += data[size / 2] & 0xFF00;
    }
    while (sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }
    return (uint16_t)(~sum);
}

vector<string> PacketSender::networkInfoFromFile()
{
    std::vector<std::string> networkInfo;
    QSettings settings("FastCam", "test");

    // Загружаем значения из настроек
    QString macSour = settings.value("macSours", "").toString();
    QString macDest = settings.value("macDest", "").toString();
    QString ipSour = settings.value("ipSours", "").toString();
    QString ipDest = settings.value("ipDest", "").toString();

    // Добавляем загруженные значения в вектор
    networkInfo.push_back(macSour.toStdString());
    networkInfo.push_back(macDest.toStdString());
    networkInfo.push_back(ipSour.toStdString());
    networkInfo.push_back(ipDest.toStdString());

    return networkInfo;
}

void PacketSender::createBasePacket(u_char *basePacket)
{
    vector<string> networkInfo = networkInfoFromFile();

    const string srcMac = networkInfo[0];
    const string dstMac = networkInfo[1];
    const string srcIp = networkInfo[2];
    const string dstIp = networkInfo[3];
    // long saddrip = inet_addr("168.254.87.106");
    // long daddrip = inet_addr("168.254.87.113");

    // Ethernet заголовок
    struct ethhdr *eth = (struct ethhdr *) basePacket;
    sscanf(srcMac.c_str(), "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
           &eth->h_source[0], &eth->h_source[1], &eth->h_source[2],
           &eth->h_source[3], &eth->h_source[4], &eth->h_source[5]);
    sscanf(dstMac.c_str(), "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
           &eth->h_dest[0], &eth->h_dest[1], &eth->h_dest[2],
           &eth->h_dest[3], &eth->h_dest[4], &eth->h_dest[5]);
    eth->h_proto = htons(0x0800); // Тип протокола (IP)

    // IP заголовок
    struct iphdr *ip = (struct iphdr *) (basePacket + ETHER_HDR_LEN);
    ip->version = 4; // IPv4
    ip->ihl = 5; // Длина заголовка
    ip->tot_len = htons(IP_HDR_LEN + UDP_HDR_LEN + DATA_LEN); // Общая длина
    ip->ttl = 128; // Время жизни
    ip->protocol = IPPROTO_UDP; // Протокол UDP
    ip->id = htons(1); // Идентификатор (например, 54321)
    ip->frag_off = 0; // Смещение фрагмента
    inet_pton(AF_INET, srcIp.c_str(), &ip->saddr); // Исходный адрес
    inet_pton(AF_INET, dstIp.c_str(), &ip->daddr); // Адрес назначения
    // ip->saddr = saddrip;
    // ip->daddr = daddrip;
    ip->check = 0;
    ip->check = calcChecksum((uint16_t *)ip, IP_HDR_LEN); // Контрольная сумма

    // UDP заголовок
    struct udphdr *udp = (struct udphdr *) (basePacket + ETHER_HDR_LEN + IP_HDR_LEN);
    udp->source = htons(50016); // Исходный порт
    udp->dest = htons(50016); // Порт назначения
    udp->len = htons(UDP_HDR_LEN + DATA_LEN); // Длина заголовка
    udp->check = 0; // Сброс контрольной суммы

    struct datahdr *hdr = (struct datahdr *) (basePacket + ETHER_HDR_LEN + IP_HDR_LEN + UDP_HDR_LEN);
    char signature[] = {0x31, 0x6D, 0x61, 0x43, 0x74, 0x73, 0x61, 0x46};
    memcpy(hdr->signature, signature, sizeof(signature));
    hdr->packetLen = DATA_LEN;
    hdr->commandNumber = 0;
}

void PacketSender::getWorkMode(u_char *packet, u_char *basePacket, UserInputData &data)
{
    size_t packetSize = ETHER_HDR_LEN + IP_HDR_LEN + UDP_HDR_LEN + DATA_LEN;
    memset(packet, 0, packetSize);
    memcpy(packet, basePacket, packetSize);

    struct datahdr *hdr = (struct datahdr *) (packet + ETHER_HDR_LEN + IP_HDR_LEN + UDP_HDR_LEN);
    hdr->commandNumber = 1;

    struct COMMAND_WORK_MODE *sendData = (struct COMMAND_WORK_MODE *) (packet + ETHER_HDR_LEN + IP_HDR_LEN + UDP_HDR_LEN + DATA_HDR_LEN);
    sendData->operationMode = data.workMode;

    struct udphdr *udp = (struct udphdr *) (packet + ETHER_HDR_LEN + IP_HDR_LEN);
    struct iphdr *ip = (struct iphdr *) (packet + ETHER_HDR_LEN);
    udp->check = udpCalcChecksum((uint16_t *)udp, UDP_HDR_LEN + DATA_LEN, ip);
}

void PacketSender::getInit(u_char *packet, u_char *basePacket, UserInputData &data)
{
    size_t packetSize = ETHER_HDR_LEN + IP_HDR_LEN + UDP_HDR_LEN + DATA_LEN;
    memset(packet, 0, packetSize);
    memcpy(packet, basePacket, packetSize);

    struct COMMAND_IMAGE_PARAM *sendData = (struct COMMAND_IMAGE_PARAM *) (packet + ETHER_HDR_LEN + IP_HDR_LEN + UDP_HDR_LEN + DATA_HDR_LEN);
    sendData->pauseFrame = data.framePause;
    sendData->packetInter = data.interframePause;
    sendData->timeExpa[0] = data.timeExpa[0];
    sendData->timeExpa[1] = data.timeExpa[1];
    sendData->timeExpa[2] = data.timeExpa[2];
    sendData->pauseAfterExpa = data.pauseAfterExpa;
    sendData->pixDelay = data.pixDelay;
    sendData->DDR_INV = data.DDR_INV;
    sendData->PIX_INV = data.DDR_INV;
    sendData->PIX_GAIN = data.PIX_GAIN;
    sendData->STROKE_ORDER = data.STROKE_ORDER;

    struct datahdr *hdr = (struct datahdr *) (packet + ETHER_HDR_LEN + IP_HDR_LEN + UDP_HDR_LEN);
    hdr->commandNumber = 2;

    struct udphdr *udp = (struct udphdr *) (packet + ETHER_HDR_LEN + IP_HDR_LEN);
    struct iphdr *ip = (struct iphdr *) (packet + ETHER_HDR_LEN);
    udp->check = udpCalcChecksum((uint16_t *)udp, UDP_HDR_LEN + DATA_LEN, ip);
}

void PacketSender::getClk(u_char *packet, u_char *basePacket, UserInputData &data)
{
    size_t packetSize = ETHER_HDR_LEN + IP_HDR_LEN + UDP_HDR_LEN + DATA_LEN;
    memset(packet, 0, packetSize);
    memcpy(packet, basePacket, packetSize);

    struct COMMAND_WRITE_PLL *sendData = (struct COMMAND_WRITE_PLL *) (packet + ETHER_HDR_LEN + IP_HDR_LEN + UDP_HDR_LEN + DATA_HDR_LEN);
    sendData->GigE_PHASE_0[0] = data.GigE_PHASE_0[0];
    sendData->GigE_PHASE_0[1] = data.GigE_PHASE_0[1];
    sendData->GigE_PHASE_0[2] = data.GigE_PHASE_0[2];
    sendData->GigE_PHASE_1[0] = data.GigE_PHASE_1[0];
    sendData->GigE_PHASE_1[1] = data.GigE_PHASE_1[1];
    sendData->GigE_PHASE_1[2] = data.GigE_PHASE_1[2];
    sendData->GigE_PHASE_2[0] = data.GigE_PHASE_2[0];
    sendData->GigE_PHASE_2[1] = data.GigE_PHASE_2[1];
    sendData->GigE_PHASE_2[2] = data.GigE_PHASE_2[2];
    sendData->GigE_PHASE_3[0] = data.GigE_PHASE_3[0];
    sendData->GigE_PHASE_3[1] = data.GigE_PHASE_3[1];
    sendData->GigE_PHASE_3[2] = data.GigE_PHASE_3[2];

    struct datahdr *hdr = (struct datahdr *) (packet + ETHER_HDR_LEN + IP_HDR_LEN + UDP_HDR_LEN);
    hdr->commandNumber = 3;

    struct udphdr *udp = (struct udphdr *) (packet + ETHER_HDR_LEN + IP_HDR_LEN);
    struct iphdr *ip = (struct iphdr *) (packet + ETHER_HDR_LEN);
    udp->check = udpCalcChecksum((uint16_t *)udp, UDP_HDR_LEN + DATA_LEN, ip);// Контрольная сумма UDP
}

void PacketSender::getLuxMess(u_char *packet, u_char *basePacket, UserInputData &data)
{
    size_t packetSize = ETHER_HDR_LEN + IP_HDR_LEN + UDP_HDR_LEN + DATA_LEN;
    memset(packet, 0, packetSize);
    memcpy(packet, basePacket, packetSize);

    struct COMMAND_LUX_PARAM *sendData = (struct COMMAND_LUX_PARAM *) (packet + ETHER_HDR_LEN + IP_HDR_LEN + UDP_HDR_LEN + DATA_HDR_LEN);
    sendData->LUX_MESS[0] = data.LUX_MESS[0];
    sendData->LUX_MESS[1] = data.LUX_MESS[1];
    sendData->LUX_MESS[2] = data.LUX_MESS[2];

    struct datahdr *hdr = (struct datahdr *) (packet + ETHER_HDR_LEN + IP_HDR_LEN + UDP_HDR_LEN);
    hdr->commandNumber = 5;

    struct udphdr *udp = (struct udphdr *) (packet + ETHER_HDR_LEN + IP_HDR_LEN);
    struct iphdr *ip = (struct iphdr *) (packet + ETHER_HDR_LEN);
    udp->check = udpCalcChecksum((uint16_t *)udp, UDP_HDR_LEN + DATA_LEN, ip);// Контрольная сумма UDP
}

void PacketSender::getLuxInit(u_char *packet, u_char *basePacket, UserInputData &data)
{
    size_t packetSize = ETHER_HDR_LEN + IP_HDR_LEN + UDP_HDR_LEN + DATA_LEN;
    memset(packet, 0, packetSize); // Обнуление пакета перед заполнением
    memcpy(packet, basePacket, packetSize);

    struct COMMAND_LUX_INIT *sendData = (struct COMMAND_LUX_INIT *) (packet + ETHER_HDR_LEN + IP_HDR_LEN + UDP_HDR_LEN + DATA_HDR_LEN);
    sendData->VABL_TST = data.VABL_TST;
    sendData->VREF = data.VREF;
    sendData->VRSTH = data.VRSTH;
    sendData->VADH = data.VADH;
    sendData->VLN = data.VLN;
    sendData->VLNA = data.VLNA;
    sendData->VLNC = data.VLNC;
    sendData->VMUX1 = data.VMUX1;
    sendData->VMUX2 = data.VMUX2;
    sendData->VOFF = data.VOFF;
    sendData->VPIX = data.VPIX;
    sendData->VTXH = data.VTXH;

    struct datahdr *hdr = (struct datahdr *) (packet + ETHER_HDR_LEN + IP_HDR_LEN + UDP_HDR_LEN);
    hdr->commandNumber = 4;

    struct udphdr *udp = (struct udphdr *) (packet + ETHER_HDR_LEN + IP_HDR_LEN);
    struct iphdr *ip = (struct iphdr *) (packet + ETHER_HDR_LEN);
    udp->check = udpCalcChecksum((uint16_t *)udp, UDP_HDR_LEN + DATA_LEN, ip);// Контрольная сумма UDP
}

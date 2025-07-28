#include "packetprocessor.h"

#include <thread>
#include <sstream>
#include <fstream>

PacketProcessor::PacketProcessor(QObject *parent) : QObject(parent) {}

void PacketProcessor::createImage(bool &continueInput)
{
    const int height = 864;

    QImage image(1344, 864, QImage::Format_Grayscale8);
    uint8_t* imageData;
    uint16_t currentFrameNumber = 0;
    while (continueInput) {
        if (imageQueue.empty()) {
            this_thread::sleep_for(chrono::milliseconds(100));
        } else {
            imageQueueMutex.lock();
            imageData = imageQueue.front();
            imageQueue.pop();
            imageQueueMutex.unlock();

            uint16_t frameNumber = *reinterpret_cast<const uint16_t*>(imageData);

            if (frameNumber != currentFrameNumber) {
                currentFrameNumber = frameNumber;
                emit imageReady(image, frameNumber);
            }

            //uint16_t pixelCount = *reinterpret_cast<const uint16_t*>(imageData.data() + 2);

            //uint16_t startX = *reinterpret_cast<const uint16_t*>(imageData.data() + 4);

            uint16_t startY = *reinterpret_cast<const uint16_t*>(imageData + 6);

            const char* pixelData = reinterpret_cast<const char*>(imageData + 32);

            memcpy(image.scanLine(startY), pixelData, 1344);

            if (startY == height - 1) {
                emit imageReady(image, frameNumber);
            }
        }
    }
    delete[] imageData;
}

void PacketProcessor::handlePacket(const char *interfaceName, bool &continueInput)
{
    pcap_t *fp;
    char errbuf[PCAP_ERRBUF_SIZE];

    fp = pcap_open_live(interfaceName, 1600, 1, 100, errbuf);
    if (fp == NULL) {
        fprintf(stderr, "\nUnable to open the adapter in savePacket. %s is not supported by WinPcap\n", interfaceName);
        QMessageBox::information(mainW,"Error", "Unable to open the adapter in savePacket", QMessageBox::Ok);
        return;
    }

    struct bpf_program filter;
    bpf_u_int32 net = 0;
    char filter_exp[] = "udp and ip src 168.254.154.113";

    if (pcap_compile(fp, &filter, filter_exp, 1, net) == -1) {
        fprintf(stderr, "Could not parse filter %s: %sn", filter_exp, pcap_geterr(fp));
        return;
    }
    if (pcap_setfilter(fp, &filter) == -1) {
        fprintf(stderr, "Could not apply filter %s: %sn", filter_exp, pcap_geterr(fp));
        return;
    }

    std::thread writer([this, &continueInput]() {
        writePacketToFile(continueInput);
    });

    std::thread createImageThread([this, &continueInput]() {
        createImage(continueInput);
    });

    countPacket = 0;
    countPacketQue = 0;

    const int dataLength = 1408;
    const int headerLength = 14 + 20 + 8; // Заголовки Ethernet, IP и UDP
    while (continueInput) {
        struct pcap_pkthdr header;
        const u_char *packet = pcap_next(fp, &header);

        if (packet == NULL) {
            continue;
        }

        countPacket++;

        std::vector<char> data;
        data.reserve(dataLength);
        data.assign(packet + headerLength, packet + headerLength + dataLength);

        uint8_t* imageData = new uint8_t[dataLength];
        memcpy(imageData, packet + headerLength + 32, dataLength);

        queueMutex.lock();
        packetQueue.push(std::move(data));
        queueMutex.unlock();

        imageQueueMutex.lock();
        imageQueue.push(imageData);
        imageQueueMutex.unlock();
    }

    writer.detach();
    createImageThread.join();
    queue<uint8_t*> emptyQueue;
    swap(imageQueue, emptyQueue);

    pcap_close(fp);
}

void PacketProcessor::writePacketToFile(bool &continueInput)
{
    ofstream file;
    QDateTime current = QDateTime::currentDateTime();
    QString qfilename = current.toString("MM-dd-hh-mm") + ".txt";
    string filename = qfilename.toStdString();

    file.open(filename, ios::binary);
    if (!file) {
        QMessageBox::information(mainW, "Error", "Error opening a file for writing", QMessageBox::Ok);
        return;
    }

    std::vector<char> packetData;

    while (continueInput || !packetQueue.empty()) {
        if (packetQueue.empty()) {
            this_thread::sleep_for(chrono::milliseconds(100));
        } else {

            ostringstream string;
            while (!packetQueue.empty()) {
                queueMutex.lock();
                packetData = std::move(packetQueue.front());
                packetQueue.pop();
                queueMutex.unlock();
                countPacketQue++;

                std::vector<char> hexChars;
                hexChars.reserve(packetData.size() * 3); // 2 символа на байт + 1 пробел

                for (unsigned char byte : packetData) {
                    hexChars.push_back("0123456789ABCDEF"[byte >> 4]);
                    hexChars.push_back("0123456789ABCDEF"[byte & 0x0F]);
                    hexChars.push_back(' ');
                }

                string.write(hexChars.data(), hexChars.size());
                string << "\n";

                if (countPacketQue % 100 == 0) {
                    file << string.str();
                    string.str("");
                    string.clear();
                }
            }
            file << string.str();
        }
    }

    file.close();
}

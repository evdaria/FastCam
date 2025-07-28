#ifndef PACKETPROCESSOR_H
#define PACKETPROCESSOR_H

#include <QObject>
#include <QMainWindow>
#include <pcap.h>
#include <vector>
#include <ws2tcpip.h>
#include <QMessageBox>
#include <mutex>
#include <queue>
#include <QObject>
#include <QImage>
#include <QDateTime>
#include <QSettings>
#include <unistd.h>
#include <Win32-Extensions.h>

using namespace std;

class PacketProcessor : public QObject {
    Q_OBJECT

signals:
    void imageReady(const QImage image, uint16_t frameNumber);

public:
    PacketProcessor(QObject *parent = nullptr);

    void handlePacket(const char *interfaceName, bool& continueInput);

private:
    QMainWindow* mainW;

    void writePacketToFile(bool& continueInput);
    void createImage(bool& continueInput);

    queue<vector<char>> packetQueue;
    mutex queueMutex;

    queue<uint8_t*> imageQueue;
    mutex imageQueueMutex;

    int countPacket;
    int countPacketQue;
};

#endif // PACKETPROCESSOR_H

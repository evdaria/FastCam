#ifndef UICONTROLLER_H
#define UICONTROLLER_H

#include <thread>

#include "packetsender.h"
#include "packetprocessor.h"

class UIController : public QObject
{
    Q_OBJECT

public:
    explicit UIController(shared_ptr<PacketProcessor> packetProcessor);
    ~UIController();

    void handleButtonClick(const char *interfaceName, int command, UserInputData &data);

private:
    shared_ptr<PacketProcessor> packetProcessor;
    unique_ptr<PacketSender> packetSender;

    bool continueInput;
    std::thread packetThread;

};

#endif // UICONTROLLER_H

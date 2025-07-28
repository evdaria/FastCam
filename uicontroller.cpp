#include "uicontroller.h"

UIController::UIController(shared_ptr<PacketProcessor> packetProcessor)
    : packetProcessor(packetProcessor), continueInput(false) {}

UIController::~UIController() {}

void UIController::handleButtonClick(const char *interfaceName, int command, UserInputData &data)
{
    if (command == 1) {
        if (data.workMode == 0) {
            packetSender->sendPacket(interfaceName, command, data);

            continueInput = false;
            if (packetThread.joinable()) {
                packetThread.join();
            }
        }
        else {
            packetSender->sendPacket(interfaceName, command, data);

            continueInput = true;
            packetThread = std::thread([this, interfaceName]() {
                packetProcessor->handlePacket(interfaceName, this->continueInput);
            });
        }
    }
    else {
        packetSender->sendPacket(interfaceName, command, data);
    }
}



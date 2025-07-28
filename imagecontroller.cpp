#include "imagecontroller.h"

void ImageController::onImageReady(const QImage image, uint16_t frameNumber)
{
    //cout << "hi on image ready" << endl;
    //mainW->displayImage(image, frameNumber);
}

ImageController::ImageController(std::shared_ptr<PacketProcessor> packetProcessor, MainWindow *mainW)
    : mainW(mainW), packetProcessor(packetProcessor)
{
    connect(packetProcessor.get(), &PacketProcessor::imageReady, this, &ImageController::onImageReady);
}

ImageController::~ImageController() {}

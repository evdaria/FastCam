#ifndef IMAGECONTROLLER_H
#define IMAGECONTROLLER_H

#include "packetprocessor.h"
#include "mainwindow.h"

class ImageController : public QObject {
    Q_OBJECT

public slots:
    void onImageReady(const QImage image, uint16_t frameNumber);

public:
    explicit ImageController(shared_ptr<PacketProcessor> packetProcessor, MainWindow *mainW);
    ~ImageController();

private:
    MainWindow *mainW;
    shared_ptr<PacketProcessor> packetProcessor;
};

#endif // IMAGECONTROLLER_H

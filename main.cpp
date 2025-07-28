#include "mainwindow.h"

#include <QApplication>
#include <pcap.h>
#include <QIcon>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("myaddico.ico"));
    a.setStyleSheet("QWidget { font-size: 13pt; }");

    MainWindow w;

    w.setWindowTitle("FastCam");
    w.show();

    const char *pcap_version = pcap_lib_version();
    printf("%s\n", pcap_version);

    return a.exec();
    return 0;
}

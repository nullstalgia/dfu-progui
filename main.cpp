#include "dfuprogui.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("dfu-progui");
    a.setOrganizationName("nullstalgia");
    a.setOrganizationDomain("nullstalgia.me");
    dfuprogui w;
    w.show();
    return a.exec();
}

#include "dfuprogui.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    dfuprogui w;
    w.show();
    return a.exec();
}

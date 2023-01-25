#include "tombola.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Tombola w;
    w.show();
    return a.exec();
}

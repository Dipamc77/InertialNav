#include "tuning_ui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    tuning_ui w;
    w.show();

    return a.exec();
}

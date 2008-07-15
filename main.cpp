#include <QApplication>

#include "bench.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Bench b;
    b.setWindowTitle(QObject::tr("R-Bench"));
    b.resize(300, 300);
    b.show();
    return app.exec();
}

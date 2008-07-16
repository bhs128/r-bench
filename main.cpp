#include <QApplication>

#include "bench.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Bench b;
    b.setWindowTitle(QObject::tr("R-Bench"));
	b.setRaySpacing(0.25);
    b.resize(300, 300);
	b.setTheta(90.0);
    b.show();
    return app.exec();
}


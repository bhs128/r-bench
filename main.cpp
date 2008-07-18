#include <QApplication>
#include <QVBoxLayout>
#include <QSlider>
#include <QWidget>

#include "bench.h"
#include "reflector.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	QWidget *window = new QWidget;
    window->setWindowTitle(QObject::tr("R-Bench"));
	
	Bench *b = new Bench();
	b->setRaySpacing(0.25);
	
	QSlider *tSlider = new QSlider(Qt::Horizontal);
	tSlider->setRange(5,175);
	QObject::connect(tSlider, SIGNAL(valueChanged(int)), 
					b, SLOT(setTheta(int)) );
	tSlider->setValue(90);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(b);
	layout->addWidget(tSlider);
	window->setLayout(layout);

	window->resize(300, 350);
    window->show();
    return app.exec();
}


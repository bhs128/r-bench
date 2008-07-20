#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QLabel>
#include <QWidget>

#include "reflector.h"
#include "bench.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	QWidget *window = new QWidget;
    window->setWindowTitle(QObject::tr("R-Bench"));
	
	Bench *b = new Bench();
	b->setRaySpacing(0.125);
	
	QLabel *tlabel = new QLabel("Theta:");
	QLabel *tvalue = new QLabel("90");
	QSlider *tSlider = new QSlider(Qt::Horizontal);
	tSlider->setRange(5,175);
	QObject::connect(tSlider, SIGNAL(valueChanged(int)), 
					b, SLOT(setTheta(int)) );
	QObject::connect(tSlider, SIGNAL(valueChanged(int)), 
					 tvalue, SLOT(setNum(int)) );
	tSlider->setValue(90);

	QLabel *alabel = new QLabel("Alpha:");
	QLabel *avalue = new QLabel("0.50");
	QSlider *aSlider = new QSlider(Qt::Horizontal);
	aSlider->setRange(1,100);
	QObject::connect(aSlider, SIGNAL(valueChanged(int)), 
					b, SLOT(setAlpha(int)) );
	QObject::connect(b, SIGNAL(alphaChanged(double)), 
					 avalue, SLOT(setNum(double)) );
	aSlider->setValue(50);
	
	QVBoxLayout *layout = new QVBoxLayout;
	
	QHBoxLayout *tlayout = new QHBoxLayout;
	tlayout->addWidget(tlabel);
	tlayout->addWidget(tSlider);
	tlayout->addWidget(tvalue);
	
	QHBoxLayout *alayout = new QHBoxLayout;
	alayout->addWidget(alabel);
	alayout->addWidget(aSlider);
	alayout->addWidget(avalue);
	
	layout->addWidget(b);
	layout->addLayout(tlayout);
	layout->addLayout(alayout);
	
	layout->setStretchFactor(b,15);
	layout->setStretchFactor(tlayout,1);
	layout->setStretchFactor(alayout,1);
	
	window->setLayout(layout);

	window->resize(300, 350);
    window->show();
    return app.exec();
}


/******************************************************************************
R-Bench - Reflector Workbench models how light reflects off mirrors of 
	various shapes
Copyright (C) 2008  Benjamin H. Schaffhausen

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
******************************************************************************/
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QLabel>
#include <QWidget>
#include <QGroupBox>
#include <QRadioButton>

#include "bench.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	QWidget *window = new QWidget;
    window->setWindowTitle(QObject::tr("R-Bench"));
	
	Bench *b = new Bench();
	b->setRaySpacing(125);
	
	QHBoxLayout *outerLayout = new QHBoxLayout;
	QVBoxLayout *sidebarLayout = new QVBoxLayout;	
	
	// Light Related Options Groupbox
	QGroupBox *inputOptionsBox = new QGroupBox("Incoming Ray Options");
	QVBoxLayout *inputOptionsLayout = new QVBoxLayout;

	QHBoxLayout *tlayout = new QHBoxLayout;
	QLabel *tlabel = new QLabel("Theta:");
	QLabel *tvalue = new QLabel("90");
	tvalue->setFixedWidth(30);
	QSlider *tSlider = new QSlider(Qt::Horizontal);
	tSlider->setRange(5,175);
	QObject::connect(tSlider, SIGNAL(valueChanged(int)), 
					b, SLOT(setTheta(int)) );
	QObject::connect(tSlider, SIGNAL(valueChanged(int)), 
					 tvalue, SLOT(setNum(int)) );
	tSlider->setValue(90);
	
	tlayout->addWidget(tlabel);
	tlayout->addWidget(tSlider);
	tlayout->addWidget(tvalue);
	inputOptionsLayout->addLayout(tlayout);
	
	QHBoxLayout *slayout = new QHBoxLayout;
	QLabel *slabel = new QLabel("Spacing:");
	QLabel *svalue = new QLabel("0.25");
	svalue->setFixedWidth(30);
	QSlider *sSlider = new QSlider(Qt::Horizontal);
	sSlider->setRange(1,50);
	QObject::connect(sSlider, SIGNAL(valueChanged(int)), 
					b, SLOT(setRaySpacing(int)) );
	QObject::connect(b, SIGNAL(spacingChanged(double)), 
					 svalue, SLOT(setNum(double)) );
	sSlider->setValue(10);
	slayout->addWidget(slabel);
	slayout->addWidget(sSlider);
	slayout->addWidget(svalue);
	inputOptionsLayout->addLayout(slayout);
	
	inputOptionsBox->setLayout(inputOptionsLayout);
	
	// Reflector Related Options Groupbox	
	QGroupBox *reflectorOptionsBox = new QGroupBox("Reflector Options");	
	QVBoxLayout *reflectorOptionsLayout = new QVBoxLayout;
	
	
	QHBoxLayout *alayout = new QHBoxLayout;
	QLabel *alabel = new QLabel("Alpha:");
	QLabel *avalue = new QLabel("0.50");
	avalue->setFixedWidth(30);
	QSlider *aSlider = new QSlider(Qt::Horizontal);
	aSlider->setRange(1,100);
	QObject::connect(aSlider, SIGNAL(valueChanged(int)), 
					b, SLOT(setAlpha(int)) );
	QObject::connect(b, SIGNAL(alphaChanged(double)), 
					 avalue, SLOT(setNum(double)) );
	aSlider->setValue(50);	
	
	alayout->addWidget(alabel);
	alayout->addWidget(aSlider);
	alayout->addWidget(avalue);
	reflectorOptionsLayout->addLayout(alayout);	
	
	QRadioButton *shape_parabola = new QRadioButton("Parabola");
	QRadioButton *shape_catenary = new QRadioButton("Catenary");
	QRadioButton *shape_semi = new QRadioButton("Semicircle");
	QObject::connect(shape_parabola, SIGNAL(toggled(bool)), 
					b, SLOT(setParabola(bool)) );
	QObject::connect(shape_catenary, SIGNAL(toggled(bool)), 
					b, SLOT(setCatenary(bool)) );
	QObject::connect(shape_semi, SIGNAL(toggled(bool)), 
					b, SLOT(setSemi(bool)) );			
	shape_catenary->toggle();
	
	QGroupBox *shapeGroupBox = new QGroupBox("Base Shape");
	QVBoxLayout *shapeLayout = new QVBoxLayout;
	shapeLayout->addWidget(shape_parabola);
	shapeLayout->addWidget(shape_catenary);
	shapeLayout->addWidget(shape_semi);
	shapeGroupBox->setLayout(shapeLayout);
	shapeGroupBox->setFlat(true);
	reflectorOptionsLayout->addWidget(shapeGroupBox);

	QHBoxLayout *minlayout = new QHBoxLayout;
	QLabel *minlabel = new QLabel("Min x:");
	QLabel *minvalue = new QLabel("0.00");
	minvalue->setFixedWidth(30);
	QSlider *minSlider = new QSlider(Qt::Horizontal);
	minSlider->setRange(-300,-1);
	QObject::connect(minSlider, SIGNAL(valueChanged(int)), 
					b, SLOT(setReflectorMin(int)) );
	QObject::connect(b, SIGNAL(reflectorMinChanged(double)), 
					 minvalue, SLOT(setNum(double)) );
	minSlider->setValue(-250);	
	minlayout->addWidget(minlabel);
	minlayout->addWidget(minSlider);
	minlayout->addWidget(minvalue);
	reflectorOptionsLayout->addLayout(minlayout);

	QHBoxLayout *maxlayout = new QHBoxLayout;
	QLabel *maxlabel = new QLabel("Max x:");
	QLabel *maxvalue = new QLabel("0.00");
	maxvalue->setFixedWidth(30);
	QSlider *maxSlider = new QSlider(Qt::Horizontal);
	maxSlider->setRange(1,300);
	QObject::connect(maxSlider, SIGNAL(valueChanged(int)), 
					b, SLOT(setReflectorMax(int)) );
	QObject::connect(b, SIGNAL(reflectorMaxChanged(double)), 
					 maxvalue, SLOT(setNum(double)) );
	maxSlider->setValue(250);	
	maxlayout->addWidget(maxlabel);
	maxlayout->addWidget(maxSlider);
	maxlayout->addWidget(maxvalue);
	reflectorOptionsLayout->addLayout(maxlayout);	

	reflectorOptionsBox->setLayout(reflectorOptionsLayout);	
	// Receiver  Related Options Groupbox	
	QGroupBox *sinkOptionsBox = new QGroupBox("Receiver Options");
	QVBoxLayout *sinkOptionsLayout = new QVBoxLayout;
	
	QHBoxLayout *onoff_layout = new QHBoxLayout;
	QCheckBox *onoffcheckbox = new QCheckBox("Enable");
	QObject::connect(onoffcheckbox, SIGNAL(stateChanged(int)), 
					b, SLOT(setReceiverEnabled(int)) );
	onoffcheckbox->setCheckState(Qt::Checked);				
	onoff_layout->addWidget(onoffcheckbox);
	sinkOptionsLayout->addLayout(onoff_layout);	
	
	QHBoxLayout *hit_layout = new QHBoxLayout;
	QLabel *hit_label = new QLabel("Light Received:");
	QLabel *hit_value = new QLabel("0");
	QLabel *mlabel = new QLabel("x");
	QObject::connect(b, SIGNAL(hitsChanged(double)), 
					 hit_value, SLOT(setNum(double)) );
	hit_layout->addWidget(hit_label);
	hit_layout->addWidget(hit_value);
	hit_layout->addWidget(mlabel);
	hit_layout->addStretch();
	sinkOptionsLayout->addLayout(hit_layout);

	QHBoxLayout *rlayout = new QHBoxLayout;
	QLabel *rlabel = new QLabel("Radius:");
	QLabel *rvalue = new QLabel("0.25");
	rvalue->setFixedWidth(30);
	QSlider *rSlider = new QSlider(Qt::Horizontal);
	rSlider->setRange(10,50);
	QObject::connect(rSlider, SIGNAL(valueChanged(int)), 
					b, SLOT(setRadius(int)) );
	QObject::connect(b, SIGNAL(radiusChanged(double)), 
					 rvalue, SLOT(setNum(double)) );
	rSlider->setValue(25);
	
	rlayout->addWidget(rlabel);
	rlayout->addWidget(rSlider);
	rlayout->addWidget(rvalue);
	sinkOptionsLayout->addLayout(rlayout);
	
	sinkOptionsBox->setLayout(sinkOptionsLayout);
	
	// add option boxes to the sidebar
	sidebarLayout->addWidget(inputOptionsBox);
	sidebarLayout->addWidget(reflectorOptionsBox);
	sidebarLayout->addWidget(sinkOptionsBox);
	sidebarLayout->addStretch();
	
	// Put everything together in the outer layout
	outerLayout->addWidget(b);
	outerLayout->addLayout(sidebarLayout);
	
	outerLayout->setStretchFactor(b,3);
	outerLayout->setStretchFactor(sidebarLayout,1);
	
	window->setLayout(outerLayout);

	window->resize(640, 480);
    window->show();
    return app.exec();
}

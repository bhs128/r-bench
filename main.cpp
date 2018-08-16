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
#include <QCheckBox>
#include <QRadioButton>
#include <QDoubleSpinBox>
#include <QComboBox>

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

	QHBoxLayout *rangelayout = new QHBoxLayout;
	QLabel *rangelabel = new QLabel("Range:");
	QDoubleSpinBox *minspin = new QDoubleSpinBox;
	minspin->setRange(-8.0,-0.05);
	minspin->setSingleStep(0.05);
	QObject::connect(minspin, SIGNAL(valueChanged(double)), 
					b, SLOT(setReflectorMin(double)) );
	minspin->setValue(-2.5);
	QDoubleSpinBox *maxspin = new QDoubleSpinBox;
	maxspin->setRange(0.05,8.0);
	maxspin->setSingleStep(0.05);
	QObject::connect(maxspin, SIGNAL(valueChanged(double)), 
					b, SLOT(setReflectorMax(double)) );
	maxspin->setValue(2.5);
	rangelayout->addWidget(rangelabel);
	rangelayout->addWidget(minspin);
	rangelayout->addWidget(maxspin);
	reflectorOptionsLayout->addLayout(rangelayout);	

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


	QHBoxLayout *rlayout = new QHBoxLayout;
	QLabel *rlabel = new QLabel("Size:");
	QLabel *rvalue = new QLabel("0.25");
	rvalue->setFixedWidth(30);
	QSlider *rSlider = new QSlider(Qt::Horizontal);
	rSlider->setRange(4,200);
	QObject::connect(rSlider, SIGNAL(valueChanged(int)), 
					b, SLOT(setSize(int)) );
	QObject::connect(b, SIGNAL(radiusChanged(double)), 
					 rvalue, SLOT(setNum(double)) );
	rSlider->setValue(25);
	rlayout->addWidget(rlabel);
	rlayout->addWidget(rSlider);
	rlayout->addWidget(rvalue);
	sinkOptionsLayout->addLayout(rlayout);	
	
	QHBoxLayout *stlayout = new QHBoxLayout;
	QLabel *stlabel = new QLabel("Angle:");
	QLabel *stvalue = new QLabel("0");
	stvalue->setFixedWidth(30);
	QSlider *stSlider = new QSlider(Qt::Horizontal);
	stSlider->setRange(0,360);
	QObject::connect(stSlider, SIGNAL(valueChanged(int)), 
					b, SLOT(setAngle(int)) );
	QObject::connect(stSlider, SIGNAL(valueChanged(int)), 
					 stvalue, SLOT(setNum(int)) );
	stSlider->setValue(180);
	stlayout->addWidget(stlabel);
	stlayout->addWidget(stSlider);
	stlayout->addWidget(stvalue);
	sinkOptionsLayout->addLayout(stlayout);

	QRadioButton *shape_pipe = new QRadioButton("Circle");
	QRadioButton *shape_panel = new QRadioButton("Line");
	QObject::connect(shape_pipe, SIGNAL(toggled(bool)), 
					b, SLOT(setPipe(bool)) );
	QObject::connect(shape_panel, SIGNAL(toggled(bool)), 
					b, SLOT(setPanel(bool)) );		
	shape_pipe->toggle();
	QGroupBox *pGroupBox = new QGroupBox("Receiver Shape");
	QVBoxLayout *pLayout = new QVBoxLayout;
	pLayout->addWidget(shape_panel);
	pLayout->addWidget(shape_pipe);
	pGroupBox->setLayout(pLayout);
	pGroupBox->setFlat(true);
	sinkOptionsLayout->addWidget(pGroupBox);
	
	QHBoxLayout *hit_layout = new QHBoxLayout;
	QLabel *hit_label = new QLabel("Power Gathered:");
	QLabel *hit_value = new QLabel("0");
	QLabel *mlabel = new QLabel("watts/unit");
	QObject::connect(b, SIGNAL(hitsChanged(double)), 
					 hit_value, SLOT(setNum(double)) );
	hit_value->setFixedWidth(30);
	hit_layout->addWidget(hit_label);
	hit_layout->addWidget(hit_value);
	hit_layout->addWidget(mlabel);
	hit_layout->addStretch();
	sinkOptionsLayout->addLayout(hit_layout);	
	
	QHBoxLayout *unit_layout = new QHBoxLayout;
	QLabel *unit_label = new QLabel("1 unit =");
	QComboBox *unitDropBox = new QComboBox();
	unitDropBox->setEditable(false);
	unitDropBox->addItem("1 Foot");
	unitDropBox->addItem("1 Meter");
	unitDropBox->addItem("1 Inch");
	QObject::connect(unitDropBox, SIGNAL(activated(int)), 
					 b, SLOT(setUnits(int)) );
	unit_layout->addWidget(unit_label);
	unit_layout->addWidget(unitDropBox);
	unit_layout->addStretch();
	sinkOptionsLayout->addLayout(unit_layout);
	
	sinkOptionsBox->setLayout(sinkOptionsLayout);
	
	// add option boxes to the sidebar
	sidebarLayout->addWidget(inputOptionsBox);
	sidebarLayout->addWidget(reflectorOptionsBox);
	sidebarLayout->addWidget(sinkOptionsBox);
	sidebarLayout->addStretch();
	
	// Put everything together in the outer layout
	outerLayout->addWidget(b);
	outerLayout->addLayout(sidebarLayout);
	
	outerLayout->setStretchFactor(b,4);
	outerLayout->setStretchFactor(sidebarLayout,1);
	
	window->setLayout(outerLayout);

	window->resize(800, 600);
    window->show();
    return app.exec();
}

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
#ifndef BENCH_H
#define BENCH_H

#include <QtGui>
#include <QWidget>
#include <QLineF>
#include <QVector>
#include <QRect>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265359
#endif

#include "reflector.h"
#include "receiver.h"

#define SCALER 1024

class Bench : public QWidget {
    Q_OBJECT

public:
    Bench(QWidget *parent = 0);

signals:
	void alphaChanged(double a); 
	void reflectorMaxChanged(double a); // to update slider label accurately
	void reflectorMinChanged(double a); 
	void spacingChanged(double a); 
	void hitsChanged(double a); 
	
public slots:
	void setTheta(int theta); // Change angle of incoming light
	void setAlpha(int alpha); // Change reflector  scaleing factor
    void setRaySpacing(int s);
	void setParabola(bool);
	void setCatenary(bool);
	void setSemi(bool);
	void setReflectorMin(int);
	void setReflectorMax(int);
	void runSimulation();
	
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
	void setLights();
	void bounce(QLineF *a_ray);	
	void drawGrid(QPainter *painter);
    void drawRays(QPainter *painter);
	
	QRect  window;
	Reflector mirror;
	Receiver sink;
	QVector<QLineF> InitialRays;
	QVector<QLineF> FinalRays;
	
	float w_right, w_left, w_top, w_bottom, w_width, w_height;
	float px_per_unit;
	float subunits_per_px;
	double 	RaySpacing;
	double	Theta; // In degrees

};

#endif

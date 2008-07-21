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
#ifndef REFLECTOR_H
#define REFLECTOR_H

#include <QtGui>
#include <QLineF>
#include <QPointF>

#ifndef SHAPES
	#define SHAPES
	#define PARA	1
	#define CAT		2
	#define SEMI 	3
#endif

class Reflector {

public:  
	Reflector(double A, double minimum, double maximum);
	
	void draw(QPainter *painter);
	QPointF intersection_coord(const QLineF *a_ray);
	bool intersects(const QLineF *a_ray);
	QLineF reflected_ray(const QLineF *a_ray);
	void setAlpha(double a);
	void setShape(const int s);
	void setFmin(const double m);
	void setFmax(const double m);
	
private:
	double func(double x);
	double d_func(double x);
	int sign(double value);
	QPointF reflect_point(QLineF *line, QPointF *point);
	
	double x_min, x_max, range, a;
	int res;
	int shape;
};

#endif

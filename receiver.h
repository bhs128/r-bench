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
#ifndef RECEIVER_H
#define RECEIVER_H

#include <QtGui>
#include <QLineF>
#include <QPointF>
#include <QRectF>
#include <cmath>

#define SCALER 1024

class Receiver {

public:  
	Receiver(double x, double y, double r);
	void reset_hits();
	void got_hit();
	int get_hits();
	bool intersects(const QLineF *a_ray);
	QPointF intersection_coord(const QLineF *a_ray);
	void draw(QPainter *painter);
	
private:
	double center_x, center_y, radius;
	int hits;   
};

#endif

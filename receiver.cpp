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
#include "receiver.h"

Receiver::Receiver(double x, double y, double s) {
	center_x = x * SCALER;
	center_y = y * SCALER;
	size = s * SCALER;
	shape=LINE;
	angle = 0.0;
	panel.setLine(center_x-size, center_y, center_x+size, center_y);
	hits = 0;
}
void Receiver::reset_hits() {
	hits = 0;  
}

void Receiver::got_hit() {
	hits++;
}

int Receiver::get_hits() {
	return hits;
}

void Receiver::setSize(double s) {
	size = s * SCALER;
	calculateLine();
}

double Receiver::getSize() {
	return size;
}

void Receiver::setShape(int s) {
	shape = s;
}
void Receiver::setCenter(double x, double y) {
	panel.translate(x-center_x,y-center_y);
	center_x = x;
	center_y = y;
}

void Receiver::setAngle(int a) {
	angle = (double) a;
	calculateLine();
}

bool Receiver::intersects(const QLineF *a_ray) {
	if(shape == LINE) {
		QPointF *answer = new QPointF();
		if(a_ray->intersect(panel, answer) == QLineF::BoundedIntersection) {
			return true;
		} else {
			return false;
		}
	}

	double x1, y1, x2, y2; 
	x1 = a_ray->x1();
	y1 = a_ray->y1();
	x2 = a_ray->x2();
	y2 = a_ray->y2();
	
	if( std::abs(a_ray->x1() - a_ray->x2()) < 0.001 ) {
		if(x1 < center_x - size) {
			return false;
		}
		if(x1 > center_x + size) {
			return false;
		}   
		if( y1 > center_y + size && y2 < center_y - size ) { // does it start above and end below?
			return true; 
		} else {
			return false;
		} 
	}
	// easy outs
	if((x1 < x2) && (center_x < x1 - size)) 
	return false;
	
	if((x1 > x2) && (center_x > x1 + size)) 
	return false;
	
	if((y1 < y2) && (center_y < y1 - size)) 
	return false;
	
	if((y1 > y2) && (center_x > y1 + size)) 
	return false;
	
	// translate points so the circle is at the origin
	x1 = x1 - center_x;
	x2 = x2 - center_x;
	y1 = y1 - center_y;
	y2 = y2 - center_y;
	
	//full equation method
	double slope = (a_ray->y2() - a_ray->y1())/(a_ray->x2() - a_ray->x1());
	double b = slope*-1*x1+y1;
	double disc = (size*size)-(b*b)+(slope*slope)*(size*size); //(slope*slope+1)*(size*size)-(b*b);
	
	if(disc > 0) {
		return true;
	} else {
		return false;
	}  
}

QPointF Receiver::intersection_coord(const QLineF *a_ray) {
	if(shape == LINE) {
		QPointF *answer = new QPointF();
		if(a_ray->intersect(panel, answer) == QLineF::BoundedIntersection) {
			return QPointF(answer->x(), answer->y());
		} 
	}

	double x1, y1, x2, y2; 
	double int1_x; //, int1_y;
	double int2_x; //, int2_y;
	
	if(std::abs(a_ray->x1() - a_ray->x2()) < 0.001) {
		return QPointF(a_ray->x1(), center_y); 
	}
	
	x1 = a_ray->x1();
	y1 = a_ray->y1();
	x2 = a_ray->x2();
	y2 = a_ray->y2();
	// translate points so the circle is at the origin
	x1 = x1 - center_x;
	x2 = x2 - center_x;
	y1 = y1 - center_y;
	y2 = y2 - center_y;
	
	//full equation method
	double m = (a_ray->y2() - a_ray->y1())/(a_ray->x2() - a_ray->x1());
	double b = m * -1 * x1 + y1;
	double disc = (size*size)-(b*b)+(m*m)*(size*size);
	int1_x = (-1*m*b+std::sqrt((float) disc))/(1+m*m);
	int2_x = (-1*m*b-std::sqrt((float) disc))/(1+m*m);
	
	return QPointF(int1_x+center_x, m*int1_x+b+center_y);  
}

void Receiver::draw(QPainter *painter) {
    QColor black(255,255,255);
    QPen stroke(black);

    QColor darkPurple(51,51,102);
    QBrush fill(darkPurple, Qt::SolidPattern);

    painter->setBrush(fill);
    painter->setPen(stroke);
	
	switch(shape) {
		case CIRCLE:
			painter->drawEllipse( QRectF(center_x-size, center_y-size, size*2.0, size*2.0) );
			break;
		case LINE:
			painter->drawLine(panel);
			break;
	}
}

void Receiver::calculateLine() {
	panel.setLine(center_x + size*std::cos(angle*0.0174532925),
				  center_y + size*std::sin(angle*0.0174532925),
				  center_x - size*std::cos(angle*0.0174532925),
				  center_y - size*std::sin(angle*0.0174532925));
}

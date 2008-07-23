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

Receiver::Receiver(double x, double y, double r) {
	center_x = x * SCALER;
	center_y = y * SCALER;
	radius = r * SCALER;
	
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

bool Receiver::intersects(const QLineF *a_ray) {
	double x1, y1, x2, y2; 
	x1 = a_ray->x1();
	y1 = a_ray->y1();
	x2 = a_ray->x2();
	y2 = a_ray->y2();
	
	if( std::abs(a_ray->x1() - a_ray->x2()) < 0.001 ) {
		if(x1 < center_x - radius) {
			return false;
		}
		if(x1 > center_x + radius) {
			return false;
		}   
		if( y1 > center_y + radius && y2 < center_y - radius ) { // does it start above and end below?
			return true; 
		} else {
			return false;
		} 
	}
	// easy outs
	if((x1 < x2) && (center_x < x1 - radius)) 
	return false;
	
	if((x1 > x2) && (center_x > x1 + radius)) 
	return false;
	
	if((y1 < y2) && (center_y < y1 - radius)) 
	return false;
	
	if((y1 > y2) && (center_x > y1 + radius)) 
	return false;
	
	// translate points so the circle is at the origin
	x1 = x1 - center_x;
	x2 = x2 - center_x;
	y1 = y1 - center_y;
	y2 = y2 - center_y;
	
	//full equation method
	double slope = (a_ray->y2() - a_ray->y1())/(a_ray->x2() - a_ray->x1());
	double b = slope*-1*x1+y1;
	double disc = (radius*radius)-(b*b)+(slope*slope)*(radius*radius); //(slope*slope+1)*(radius*radius)-(b*b);
	
	if(disc > 0) {
		return true;
	} else {
		return false;
	}  
}

QPointF Receiver::intersection_coord(const QLineF *a_ray) {
	double x1, y1, x2, y2; 
	double int1_x, int1_y;
	double int2_x, int2_y;
	
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
	double disc = (radius*radius)-(b*b)+(m*m)*(radius*radius);
	int1_x = (-1*m*b+std::sqrt((float) disc))/(1+m*m);
	int2_x = (-1*m*b-std::sqrt((float) disc))/(1+m*m);
	
	return QPointF(int1_x+center_x, m*int1_x+b+center_y);  
}

void Receiver::draw(QPainter *painter) {
    QColor darkGrey(51,51,102);
    QBrush fill(darkGrey, Qt::SolidPattern);
    painter->setBrush(fill);
	
	painter->drawEllipse( QRectF(center_x-radius, center_y-radius, radius*2.0, radius*2.0) );
}
 
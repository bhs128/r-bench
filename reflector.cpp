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
#include <QtGui>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265359
#endif

#include "reflector.h"

Reflector::Reflector(double A, double minimum, double maximum) {
	a = A;
	x_min = minimum * SCALER;
	x_max = maximum * SCALER;
	range = x_max - x_min;
	res = 1000;
	shape = CAT;
}

double Reflector::func(double x) { //one (or more) equations describing the reflector shape
	double radius;
	double answer;
	x = x / (double) SCALER;
	
	switch(shape) {
		case PARA:
			answer = a*(x*x); // Parabola
			break;
		case SEMI:
			radius = a * -6.0 + 9.0;
			answer = -1 * std::sqrt((float) (radius*radius-x*x))+radius; // semicircle
			break;
		case CAT:
		default:
			answer = a*std::cosh(x)-a; // Catenary
	}
	
	return answer * SCALER;	
}

double Reflector::d_func(double x) { // the derivative function of func()
	double radius;
	double answer;
	x = x / (double) SCALER;	

	switch(shape) {
		case PARA:
			answer = a*2*x; // Parabola	
			break;
		case SEMI:
			radius = a * -6.0 + 9.0;
			answer = x / std::sqrt((float) (radius*radius-x*x));// semicircle
			break;
		case CAT:
		default:
			answer = a*std::sinh(x); // Catenary
	}
	
	return answer;
}

void Reflector::setAlpha(double alpha) {
	a = alpha;
}

void Reflector::setShape(const int s) {
	shape = s;
}

void Reflector::setFmin(const double min) {
	x_min = min * SCALER;
}

void Reflector::setFmax(const double max) {
	x_max = max * SCALER;
}

double Reflector::fMin() {
	return x_min / (double) SCALER;
}

double Reflector::fMax() {
	return x_max / (double) SCALER;
}

void Reflector::draw(QPainter *painter) {
    QColor darkGrey(196,196,196);
    QPen rPen(darkGrey);
    painter->setPen(rPen);
	
	float x1 = x_min;
	float y1 = func(x_min);
	float x2, y2;
	for(double x = x_min; x <= x_max; x = x + (range / res) ) { 
		x2 = x;
		y2 = func(x);
		painter->drawLine( QLineF(x1, y1, x2, y2) );
		x1 = x2;
		y1 = y2;
	}
}

bool Reflector::intersects(const QLineF *a_ray) {
	if(std::abs(a_ray->x1() - a_ray->x2()) < 0.001) { // is vertical
		if(a_ray->x1() < x_min) {
			return false;
		}
		if(a_ray->x1() > x_max) {
			return false;
		}   
		if( a_ray->y1() > func(a_ray->x1()) && a_ray->y2() < func(a_ray->x2()) ) { // does it start above and end below?
			return true; 
		} else {
			return false;
		}
	} else { // not a vertical ray..
		double ax1, ay1, ax2, ay2; //a_ray's start points
		int last_sign, cur_sign;
		double am = (a_ray->y2() - a_ray->y1())/(a_ray->x2() - a_ray->x1());
		
		ax1 = a_ray->x1();
		ay1 = a_ray->y1();
		ax2 = a_ray->x2();
		ay2 = a_ray->y2();

		if(ax1 < ax2) {
			last_sign = sign(func(ax1+0.01) - (am*((ax1+0.01)-ax1)+ay1) );
			for(double x = (ax1+0.01); x <= x_max; x += range/1000.0) {
				cur_sign = sign(func(x) - (am*(x-ax1)+ay1));
				if(cur_sign != last_sign) { // change in sign means interection
					if(x <= x_max && x >= x_min) {
						return true;
					}
				}
				last_sign = cur_sign;
			}
		} else {
			last_sign = sign(func(ax1-0.01) - (am*((ax1-0.01)-ax1)+ay1) );
			for(double x = (ax1-0.01); x >= x_min; x -= range/1000.0) {
				cur_sign = sign(func(x) - (am*(x-ax1)+ay1));
				if(cur_sign != last_sign) { // change in sign means interection
					if(x <= x_max && x >= x_min) {
						return true;
					}
				}
				last_sign = cur_sign;
			}
		}
		return false;
	} // end non-vertical intersection test
}

QPointF Reflector::intersection_coord(const QLineF *a_ray) {
	double xi, yi;
	
	if(a_ray->x1() == a_ray->x2()) { // is vertical
		return QPointF( a_ray->x1() , func(a_ray->x1()) );
	} else {
		double ax1, ay1, ax2, ay2; //a_ray's points
		double xp1, xp2; // x point 1 and two, intersection areas
		double dist_xp1, dist_xp2; // distance to possible intersection points from the starting point of ray
		double am = (a_ray->y2() - a_ray->y1())/(a_ray->x2() - a_ray->x1());
		int last_sign, cur_sign;

		ax1 = a_ray->x1();
		ay1 = a_ray->y1();
		ax2 = a_ray->x2();
		ay2 = a_ray->y2();  
		xp1 = xp2 = -1000000.0;
		
		if(ax1 < ax2) { // what direction is the ray facing?
			last_sign = sign(func(ax1+0.001) - (am*((ax1+0.001)-ax1)+ay1) );
			for(double x = (ax1+0.01); x <= x_max; x += range/1000.0) { //trace the ray from just after its origin
				cur_sign = sign(func(x) - (am*(x-ax1)+ay1));
				//stroke(255,0,0);
				//line(map_x(x), map_y(0.0), map_x(x), map_y(func(x) - (am*(x-ax1)+ay1)) );
				if(cur_sign != last_sign) { // change in sign means an interection
					if(x> x_min && x < x_max) { // is the intersection on the reflector?
						if(xp1 <= -100000.0) { // xp1 is empty , fill it- otherwise fill xp2
							xp1 = x;
						} else {
							xp2 = x;
						}
					}
				}
				last_sign = cur_sign;
			}
		} else { // ray traveling right to left
			last_sign = sign(func(ax1-0.001) - (am*((ax1-0.001)-ax1)+ay1) );
			for(double x = (ax1-0.01); x >= x_min; x -= range/1000.0) {
				cur_sign = sign(func(x) - (am*(x-ax1)+ay1));
				//stroke(255,0,0);
				//line(map_x(x), map_y(0.0), map_x(x), map_y(func(x) - (am*(x-ax1)+ay1)) );
				if(cur_sign != last_sign) { // change in sign means an interection
					if(x> x_min && x < x_max) { // is the intersection on the reflector?
						if(xp1 <= -100000.0) { // xp1 is empty , fill it- otherwise fill xp2
							xp1 = x;
						} else {
							xp2 = x;
						}
					}
				}
				last_sign = cur_sign;
			}        
		}
		
		// the if() below needs to get the intersection point
		if(xp2 <= -1000.0) { // xp2 is still empty- xp1 is the only intersection
			xi = xp1;
			yi = func(xp1);
		} else { // there are two
			dist_xp1 = sqrt(pow((float) (ax1 - xp1),2) + pow((float) (ay1 - func(xp1)), 2));
			dist_xp2 = sqrt(pow((float) (ax1 - xp2),2) + pow((float) (ay1 - func(xp2)), 2));
			if(dist_xp1 < dist_xp2) { // use the closer of the two
				xi = xp1;
				yi = func(xp1);
			} else {
				xi = xp2;
				yi = func(xp2);
			}
		}
		//stroke(255,0,0);
		//line(map_x(xi), map_y(0.0), map_x(xi), map_y(func(xi)) );
		return QPointF(xi,yi);
	}
}

QLineF Reflector::reflected_ray(const QLineF *a_ray) { 
	double ax1, ay1, ax2, ay2; //a_ray's points
	double bx1, by1, bx2, by2; 
	
	ax1 = a_ray->x1();
	ay1 = a_ray->y1();
	ax2 = a_ray->x2();
	ay2 = a_ray->y2();
	
	if(std::abs(a_ray->x1() - a_ray->x2()) < 0.001) { // is vertical
		//first point is coord of intersection 
		bx1 = ax1;       
		by1 = func(ax1); 
		
		float shift = (ax1>0)?M_PI/2:-1*M_PI/2;
		float theta = std::atan((float)d_func(ax1))*2-shift;
		//double r_slope = std::tan(theta);
		double d_x = SCALER*15*std::cos(theta);
		double d_y = SCALER*15*std::sin(theta);
		
		if(ax1<=0) {
			bx2 = ax1 + d_x;
			by2 = func(ax1) + d_y;
		} else {
			bx2 = ax1 - d_x;
			by2 = func(ax1) - d_y;
		}
		
		return QLineF(bx1, by1, bx2, by2);
	} else { // not a vertical ray
		QPointF  r_start, r_end; // coords of reflected endpoints of a_ray
		QPointF  isect;
		double xi, yi;
		QLineF tangent;
		double fm; //function slope at intersection
		
		isect = intersection_coord(a_ray);
		xi = isect.x();
		yi = isect.y();
		
		fm = d_func(xi);
		tangent = QLineF(xi-1.0, fm*((xi-1.0)-xi)+yi, xi+1.0, fm*((xi+1.0)-xi)+yi );
		//stroke(128);
		//tangent.draw();
		// reflect the start and end points over the tangent ray
		r_start = reflect_point( &tangent, new QPointF(ax1, ay1) );
		r_end = reflect_point( &tangent, new QPointF(ax2, ay2) );
		
		// reflected start and end points are the reflected ray, return it-
		return QLineF( r_start.x(), r_start.y(), r_end.x(), r_end.y() );
	}
}

QPointF Reflector::reflect_point(QLineF *line, QPointF *p) {
	double ml = (line->y2() - line->y1())/(line->x2() - line->x1());
	double mr = 1.0 / ml * -1.0; 
	double px, py; // starting point
	double xi, yi; // intersection point
	double xp, yp; // reflected point, x & y prime

	px = p->x();
	py = p->y();

	xi = (mr*px - py - ml*line->x1() + line->y1())/(mr-ml);
	yi = ml*(xi - line->x1()) + line->y1();

	//line(map_x(px), map_y(py), map_x(xi), map_y(yi)); 
	xp = px + 2.0*(xi - px);
	yp = py + 2.0*(yi - py);

	return QPointF(xp, yp);

}

int Reflector::sign(double value) {
	if(value < 0) 
		return -1;
	else
		return 1;
}
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265359
#endif

#include "reflector.h"

double x_min, x_max, range, a;
QLineF gate_line;
int res = 100;

Reflector::Reflector(double A, double minimum, double maximum) {
	a = A;
	x_min = minimum;
	x_max = maximum;
	range = x_max - x_min;
	res = 100;
	gate_line = new QLineF(x_min, func(x_min), x_max, func(x_max));

}

double Reflector::func(double x) { //one (or more) equations describing the reflector shape
	return a*std::cosh(x)-a; // Catenary
	//return a*(x*x); // Parabola
	//return -1 * sqrt((float) (a*a-x*x))+a; // semicircle
}

double Reflector::d_func(double x) { // the derivative function of func()
	return a*std::sinh(x); // Catenary
	//return a*2*x; // Parabola
	//return x / sqrt((float) (a*a-x*x));// semicircle
}

void Reflector::draw(QPainter *painter) {
    QColor darkGrey(64,64,64);
    QPen rPen(darkGrey);
    painter->setPen(rPen, 0.5);
	
	float x1 = map_x(x_min);
	float y1 = map_y(func(x_min));
	float x2, y2;
	for(double x = x_min; x <= x_max; x = x + (range / res) ) { 
		x2 = map_x(x);
		y2 = map_y(func(x));
		painter->drawLine(x1, y1, x2, y2);
		x1 = x2;
		y1 = y2;
	}
}

bool Reflector::enters(QLineF a_ray, float theta) {
	double ax1, ay1, ax2, ay2; //a_ray's start points
	double lx, rx; 
	ax1 = a_ray.x1();
	ay1 = a_ray.y1();
	ax2 = a_ray.x2();
	ay2 = a_ray.y2();

	lx = (ay1 - gate_line.y1()) / tan(theta);
	rx = (ay1 - gate_line.y2()) / tan(theta);
	
	if(ay1 < gate_line.y1() && ay1 < gate_line.y2()) { // can't enter if you start below the gate line
		return false;
	} 
	if(ax1 > lx && ax1 < rx) {
		return true;
	} else {
		return false;
	}
}

bool Reflector::intersects(QLineF a_ray) {
	if(a_ray.is_vertical()) {
		if(a_ray.x1() < x_min) {
			return false;
		}
		if(a_ray.x1() > x_max) {
			return false;
		}   
		if( a_ray.y1() > func(a_ray.x1()) && a_ray.y2() < func(a_ray.x1()) ) { // does it start above and end below?
			return true; 
		} else {
			return false;
		}
	} else { // not a vertical ray..
		double ax1, ay1, ax2, ay2; //a_ray's start points
		int last_sign, cur_sign;
		double am = a_ray.get_slope();
		
		ax1 = a_ray.x1();
		ay1 = a_ray.y1();
		ax2 = a_ray.x2();
		ay2 = a_ray.y2();

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

QPointF Reflector::intersection_coord(QLineF a_ray) {
	double xi, yi;
	
	if(a_ray.is_vertical()) {
		return new QPointF( a_ray.x1() , func(a_ray.x1()) );
	} else {
		double ax1, ay1, ax2, ay2; //a_ray's points
		double xp1, xp2; // x point 1 and two, intersection areas
		double dist_xp1, dist_xp2; // distance to possible intersection points from the starting point of ray
		double am = a_ray.get_slope();
		int last_sign, cur_sign;

		ax1 = a_ray.x1();
		ay1 = a_ray.y1();
		ax2 = a_ray.x2();
		ay2 = a_ray.y2();  
		xp1 = xp2 = -10000.0;
		
		if(ax1 < ax2) { // what direction is the ray facing?
			last_sign = sign(func(ax1+0.001) - (am*((ax1+0.001)-ax1)+ay1) );
			for(double x = (ax1+0.01); x <= x_max; x += range/1000.0) { //trace the ray from just after its origin
				cur_sign = sign(func(x) - (am*(x-ax1)+ay1));
				//stroke(255,0,0);
				//line(map_x(x), map_y(0.0), map_x(x), map_y(func(x) - (am*(x-ax1)+ay1)) );
				if(cur_sign != last_sign) { // change in sign means an interection
					if(x> x_min && x < x_max) { // is the intersection on the reflector?
						if(xp1 <= -1000.0) { // xp1 is empty , fill it- otherwise fill xp2
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
						if(xp1 <= -1000.0) { // xp1 is empty , fill it- otherwise fill xp2
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
		return new QPointF(xi,yi);
	}
}

QLineF Reflector::reflected_ray(QLineF a_ray) { 
	double ax1, ay1, ax2, ay2; //a_ray's points
	double bx1, by1, bx2, by2; //reflected ray's points
	
	ax1 = a_ray.x1();
	ay1 = a_ray.y1();
	ax2 = a_ray.x2();
	ay2 = a_ray.y2();
	
	if(a_ray.is_vertical()) { // only works for vertical incoming rays
		//first point is coord of intersection 
		bx1 = ax1;       
		by1 = func(ax1); 
		
		float shift = (ax1>0)?PI/2:-1*PI/2;
		float theta = atan((float)d_func(ax1))*2-shift;
		double r_slope = tan(theta);
		double d_x = 10*cos(theta);
		double d_y = 10*sin(theta);
		
		if(ax1<=0) {
			bx2 = ax1 + d_x;
			by2 = func(ax1) + d_y;
		} else {
			bx2 = ax1 - d_x;
			by2 = func(ax1) - d_y;
		}
		
		return new QLineF(bx1, by1, bx2, by2, a_ray.get_pwr() + 1);
	} else { // not a vertical ray
		Coord r_start, r_end; // coords of reflected endpoints of a_ray
		Coord isect;
		double xi, yi;
		Ray tangent;
		double fm; //function slope at intersection
		
		isect = intersection_coord(a_ray);
		xi = isect.getX();
		yi = isect.getY();
		
		fm = d_func(xi);
		tangent = new Ray(xi-1.0, fm*((xi-1.0)-xi)+yi, xi+1.0, fm*((xi+1.0)-xi)+yi );
		//stroke(128);
		//tangent.draw();
		// reflect the start and end points over the tangent ray
		r_start = tangent.reflect_point(new Coord(ax1, ay1));
		r_end = tangent.reflect_point(new Coord(ax2, ay2));
		
		// reflected start and end points are the reflected ray, return it-
		return new QLineF( r_start.getX(), r_start.getY(), r_end.getX(), r_end.getY(), a_ray.get_pwr() + 1);
	}
}

int Reflector::sign(double value) {
	if(value < 0) 
		return -1;
	else
		return 1;
}
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
#include "reflector.h"

Reflector::Reflector(double A, double minimum, double maximum) {
	a = A;
	x_min = minimum * SCALER;
	x_max = maximum * SCALER;
	range = x_max - x_min;
	res = 200;
	shape = CAT;
	rebuildPolyline();
}

double Reflector::func(double x) { //one (or more) equations describing the reflector shape
	double radius;
	double answer;
	double max_radius = ((std::abs(x_min)>x_max)?std::abs(x_min):x_max) / (double) SCALER;
	x = x / (double) SCALER;
	
	switch(shape) {
		case PARA:
			answer = a*(x*x); // Parabola
			break;
		case SEMI:
			radius = a * (-4.0 * max_radius) + max_radius*5.01;
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
	double max_radius = ((std::abs(x_min)>x_max)?std::abs(x_min):x_max) / (double) SCALER;
	x = x / (double) SCALER;	

	switch(shape) {
		case PARA:
			answer = a*2*x; // Parabola	
			break;
		case SEMI:
			radius = a * (-4.0 * max_radius) + max_radius*5.01;
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
	rebuildPolyline();
}

void Reflector::setShape(const int s) {
	shape = s;
	rebuildPolyline();
}

void Reflector::setFmin(const double min) {
	x_min = min * SCALER;
	range = x_max - x_min;
	rebuildPolyline();
}

void Reflector::setFmax(const double max) {
	x_max = max * SCALER;
	range = x_max - x_min;
	rebuildPolyline();
}

double Reflector::fMin() {
	return x_min / (double) SCALER;
}

double Reflector::fMax() {
	return x_max / (double) SCALER;
}
void Reflector::reset_hits() {
	hits = 0;
}

void Reflector::got_hit() {
	hits++;
}

int Reflector::get_hits() {
	return hits;
}
	
void Reflector::rebuildPolyline() {
	segments.clear();
	segments.reserve(res);
	double step = (x_max - x_min) / res;
	double prevX = x_min;
	double prevY = func(x_min);
	for (int i = 1; i <= res; i++) {
		double x = x_min + i * step;
		double y = func(x);
		segments.append(QLineF(prevX, prevY, x, y));
		prevX = x;
		prevY = y;
	}
}

void Reflector::draw(QPainter *painter) {
    QColor darkGrey(196,196,196);
    QPen rPen(darkGrey);
    painter->setPen(rPen);
	painter->drawLines(segments);
}

bool Reflector::findIntersection(const QLineF *ray, QPointF &hitPoint) {
	QPointF closest;
	double minDist = -1;
	bool found = false;
	double originX = ray->x1();
	double originY = ray->y1();

	for (const QLineF &seg : segments) {
		QPointF pt;
		if (ray->intersects(seg, &pt) == QLineF::BoundedIntersection) {
			double dx = pt.x() - originX;
			double dy = pt.y() - originY;
			double dist = dx*dx + dy*dy;
			if (dist < 1.0) continue; // skip hits at ray origin (bounced rays)
			if (!found || dist < minDist) {
				minDist = dist;
				closest = pt;
				found = true;
			}
		}
	}

	if (found) {
		hitPoint = closest;
	}
	return found;
}

QLineF Reflector::reflected_ray(const QLineF *a_ray, const QPointF &hitPoint) { 
	double ax1, ay1, ax2, ay2; //a_ray's points
	double bx1, by1, bx2, by2; 
	
	ax1 = a_ray->x1();
	ay1 = a_ray->y1();
	ax2 = a_ray->x2();
	ay2 = a_ray->y2();
	
	double xi = hitPoint.x();
	double yi = hitPoint.y();
	
	if(std::abs(a_ray->x1() - a_ray->x2()) < 0.001) { // is vertical
		bx1 = ax1;       
		by1 = yi; 
		
		float shift = (ax1>0)?M_PI/2:-1*M_PI/2;
		float theta = std::atan((float)d_func(ax1))*2-shift;
		double d_x = SCALER*50*std::cos(theta);
		double d_y = SCALER*50*std::sin(theta);
		
		if(ax1<=0) {
			bx2 = ax1 + d_x;
			by2 = yi + d_y;
		} else {
			bx2 = ax1 - d_x;
			by2 = yi - d_y;
		}
		
		return QLineF(bx1, by1, bx2, by2);
	} else { // not a vertical ray
		QPointF  r_start, r_end;
		QLineF tangent;
		double fm; //function slope at intersection
		
		fm = d_func(xi);
		tangent = QLineF(xi-1.0, fm*((xi-1.0)-xi)+yi, xi+1.0, fm*((xi+1.0)-xi)+yi );
		// reflect the start and end points over the tangent ray
		QPointF startPt(ax1, ay1);
		QPointF endPt(ax2, ay2);
		r_start = reflect_point( &tangent, &startPt );
		r_end = reflect_point( &tangent, &endPt );
		
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

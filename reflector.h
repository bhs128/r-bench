#ifndef REFLECTOR_H
#define REFLECTOR_H

#include <QtGui>
#include <QLineF>
#include <QPointF>

class Reflector {

public:  
	Reflector(double A, double minimum, double maximum);
	
	void draw(QPainter *painter);
	QPointF intersection_coord(QLineF a_ray);
	bool enters(QLineF a_ray, float theta);
	bool intersects(QLineF a_ray);
	QLineF reflected_ray(QLineF a_ray);
	
private:
	double func(double x);
	double d_func(double x);
	int sign(double value);
	
	double x_min, x_max, range, a;
	QLineF gate_line;
	int res;
};

#endif

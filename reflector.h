#ifndef REFLECTOR_H
#define REFLECTOR_H

#include <QtGui>
#include <QLineF>
#include <QPointF>

class Reflector {

public:  
	Reflector(double A, double minimum, double maximum);
	
	void draw(QPainter *painter);
	QPointF intersection_coord(const QLineF *a_ray);
	bool enters(QLineF a_ray, float theta);
	bool intersects(const QLineF *a_ray);
	QLineF reflected_ray(const QLineF *a_ray);
	
private:
	double func(double x);
	double d_func(double x);
	int sign(double value);
	QPointF reflect_point(QLineF *line, QPointF *point);
	
	double x_min, x_max, range, a;
	QLineF gate_line;
	int res;
};

#endif

#ifndef BENCH_H
#define BENCH_H

#include <QWidget>
#include <QLineF>
#include <QVector>
#include <QRect>

#include "reflector.h"

class Bench : public QWidget {
    Q_OBJECT

public:
    Bench(QWidget *parent = 0);

signals:
	void alphaChanged(double a); 
	
public slots:
	void setTheta(int theta); // Change angle of incoming light
	void setAlpha(int alpha); // Change reflector  scaleing factor
    void setRaySpacing(double s);
	void runSimulation();
	
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
	void setLights(double theta);
	void bounce(QLineF *a_ray);	
	void drawGrid(QPainter *painter);
    void drawRays(QPainter *painter);
	QRect  window;
	Reflector mirror;

};

#endif

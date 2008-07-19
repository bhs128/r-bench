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

//signals:

public slots:
	void setTheta(int theta);
    void setRaySpacing(double s);
	
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
	void setLights(double theta);
	
	void drawGrid(QPainter *painter);
    void drawRays(QPainter *painter);
	QRect  window;
	Reflector *mirror;

};

#endif

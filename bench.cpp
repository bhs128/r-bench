#include <QtGui>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265359
#endif

#include "bench.h"

double 	RaySpacing;
double	Theta; // In degrees
QVector<QLineF> InitialRays;
QVector<QLineF> FinalRays;

Bench::Bench(QWidget *parent) : QWidget(parent) {
	window = QRectF(-5.0, 5.0, 10.0, 10.0);
	RaySpacing = 1.0;
}

void Bench::setRaySpacing(double s) {
	RaySpacing = s;
}

void Bench::setTheta(int t) {
	Theta = (double) t;
	setLights(Theta);
}
void Bench::mousePressEvent(QMouseEvent *event)
{
	QPointF point = event->pos() - rect().center();
	//setLights(90.0);
	//InitialRays.append(QLineF(0,0,point.x(), point.y() ));
    update();
}

void Bench::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    //painter.setRenderHint(QPainter::Antialiasing, true);

    //painter.setViewport((width() - side) / 2, (height() - side) / 2, side, side);
    painter.setWindow((int) window.x(), (int) window.y(), (int) window.width(), (int) window.height() );
	setLights(Theta);
	
    draw(&painter);
}

void Bench::draw(QPainter *painter)
{
    QColor niceYellow(0,0,0);
    QPen thinPen(niceYellow);
	
    painter->setPen(thinPen);
    painter->drawLines(InitialRays); 
	painter->drawRect(window);
}

void Bench::setLights(double theta) {

    InitialRays.resize(0);  // reset array to null
	
	if(theta == 90.0) {
		for(double x = window.left(); x < window.right(); x += RaySpacing) {
			InitialRays.append( QLineF(x, window.top(), x, window.bottom() ) );
		}
	} else {
		float t = theta * 0.0174532925; // ((theta < 90.0) ? theta : 180.0 - theta) * 0.0174532925;
		float x_inc = RaySpacing / std::sin(t);
		float y_inc = RaySpacing / std::cos(t);
		float c_x1 = window.right(); // (theta < 90.0) ? window.right() : window.left();
		float c_y1 = window.top();
		float c_x2 = c_x1 - 10.0 * std::cos(t); // ((theta < 90.0) ? c_x1 - 10.0: c_x1 + 10.0) * std::cos(t);
		float c_y2 = c_y1 - 10.0 * std::sin(t);
		
		InitialRays.append( QLineF( c_x1, c_y1, c_x2, c_y2) );
		int rays_across = (int) (window.width() / x_inc);
		int rays_down   = (int) (window.height() / y_inc);
		
		for(int idx = 0; idx <= rays_across; idx++) {
			double shift;
			if(theta < 90.0) {
				shift =  -1.0 * idx * x_inc;
			} else {
				shift = (double) idx * x_inc;
			}
			InitialRays.append( QLineF((float) idx*x_inc-2.0, 1.0,
									   (float) idx*x_inc-2.0, -1.0) );
			InitialRays.append( QLineF((float) c_x1 + shift, (float) c_y1, (float) c_x2 + shift, (float) c_y2) );
		} 
		for(int idx = 1; idx <= rays_down; idx++) {
			InitialRays.append( QLineF(c_x1, c_y1 - idx*y_inc, c_x2, c_y2 - idx*y_inc) );
			InitialRays.append( QLineF(-1.0, (float) (idx*y_inc-2.0), 1.0, (float) (idx*y_inc-2.0) ) );
		} 
	}
	update(); // schedule a repaint of new rays
}
 
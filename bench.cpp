#include <QtGui>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265359
#endif

#include "bench.h"

double 	RaySpacing;
double	Theta; // In degrees
float w_right = 5.0;
float w_left = -5.0;
float w_top, w_bottom, w_width, w_height;
float px_per_unit;

QVector<QLineF> InitialRays;
QVector<QLineF> FinalRays;

Bench::Bench(QWidget *parent) : QWidget(parent) {
	w_right = 5.0;
	w_left = -5.0;

	RaySpacing = 1.0;
	mirror = new Reflector(0.5, -2.0, 2.0);
}

void Bench::setRaySpacing(double s) {
	RaySpacing = s;
}

void Bench::setTheta(int t) {
	Theta = (double) t;
	setLights(Theta);
}

void Bench::mousePressEvent(QMouseEvent *event) {
	QPointF point = event->pos() - rect().center();
	//setLights(90.0);
	//InitialRays.append(QLineF(0,0,point.x(), point.y() ));
    update();
}

void Bench::paintEvent(QPaintEvent * /* event */) {
	QMatrix reflectionMatrix(1, 0, 0, -1, 0.0, 0.0); // Defines a reflection over the x-axis
    QPainter painter(this);
	painter.setMatrix(reflectionMatrix);

    painter.setRenderHint(QPainter::Antialiasing, true);
	w_width = w_right - w_left;
	px_per_unit = width() / w_width;
	w_bottom = -1.0;
	w_top = height() / px_per_unit + w_bottom;
	w_height = w_top - w_bottom;
	
	window = QRect((int) w_left, -1 * (int) w_top, (int) w_width, (int) w_height);

    painter.setWindow(window);
	setLights(Theta);
	drawGrid(&painter);	
    drawRays(&painter);
	mirror.draw(&painter);
}

void Bench::drawRays(QPainter *painter) {
    QColor niceYellow(255,192,0);
    QPen thinPen(niceYellow);
    painter->setPen(thinPen);

    painter->drawLines(InitialRays); 
}

void Bench::drawGrid(QPainter *painter) {
	QColor niceBlue(0,0,255);
    QPen   gridPen(niceBlue);
	
	painter->fillRect( (int) w_left, (int) w_bottom,(int) w_width, (int) w_height, QColor(255,255,255)); 
	painter->setPen(gridPen);
	// Horizontal X-Axis
	painter->drawLine( QLineF(w_left, 0.0, w_right, 0.0) );
	// Vertical Y-axis
	painter->drawLine( QLineF(0.0, w_top, 0.0, w_bottom) );
	
	//painter->drawPoint(-1,-1);
}

void Bench::setLights(double theta) {

    InitialRays.resize(0);  // reset array to null
	
	if(theta == 90.0) {
		for(double x = w_left; x < w_right; x += RaySpacing) {
			InitialRays.append( QLineF(x, w_top, x, w_bottom ) );
		}
	} else {
		float t = ((theta < 90.0) ? theta : 180.0 - theta) * 0.0174532925;
		float x_inc = RaySpacing / std::sin(t);
		float y_inc = RaySpacing / std::cos(t);
		float c_x1 = (theta < 90.0) ? w_right : w_left;
		float c_y1 = w_top;
		float c_x2 = c_x1 + (((theta < 90.0) ? -15 : 15 ) * std::cos(t));
		float c_y2 = c_y1 - 15.0 * std::sin(t);
		
		InitialRays.append( QLineF( c_x1, c_y1, c_x2, c_y2) );
		int rays_across = (int) (w_width / x_inc);
		int rays_down   = (int) (w_height / y_inc);
		
		for(int idx = 1; idx <= rays_across; idx++) {
			double shift;
			if(theta < 90.0) {
				shift =  -1.0 * idx * x_inc;
			} else {
				shift = (double) idx * x_inc;
			}
			InitialRays.append( QLineF((float) c_x1 + shift, (float) c_y1, (float) c_x2 + shift, (float) c_y2) );
		} 
		for(int idx = 1; idx <= rays_down; idx++) {
			InitialRays.append( QLineF(c_x1, c_y1 - idx*y_inc, c_x2, c_y2 - idx*y_inc) );
		} 
	}
	update(); // schedule a repaint of new rays
}
 
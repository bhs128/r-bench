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
	window = QRectF(-5.0, -5.0, 10.0, 10.0);
	RaySpacing = 1.0;
}

void Bench::setRaySpacing(double s) {
	RaySpacing = s;
}

void Bench::setTheta(double t) {
	Theta = t;
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

    int side = qMin(width(), height());

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
	if(theta == 90.0) {
		for(double x = window.left(); x < window.right(); x += RaySpacing) {
			InitialRays.append(QLineF(x, window.top(), x, window.bottom() ) );
		}
	}
	//update();
}
/*
void set_lightsdouble t, double spacing) {
	float px_spacing = (float)(w_px*spacing/x_range);
    float num_rays = w_px / px_spacing;
    Ray temp;
    
    InitialRays.resize(0);  // reset array to null
  
    if(t == 90.0) {
      for(int idx = 0; idx < num_rays; idx++) {
        temp = new Ray(idx*spacing+x_min, y_max, idx*spacing+x_min, y_min, 0);
        //temp.draw();
        input_array[idx] = temp;
      }
    } else if(t < 90.0) {
      t = radians((float)t);
      double x_inc = spacing / sin((float)t);
      double y_inc = spacing / cos((float)t);
  
      double c_x1 = x_max;
      double c_y1 = y_max;
      double c_x2 = c_x1 - 15 * cos((float) t);
      double c_y2 = c_y1 - 15 * sin((float) t);
      int ray_num = 0;
      int rays_across = (int) (x_range / x_inc);
      int rays_down   = (int) (y_range / y_inc);
      
      for(int idx = 0; idx <= rays_across; idx++) {
        temp = new Ray(c_x1 - idx*x_inc, c_y1, c_x2 - idx*x_inc, c_y2, 0);
        input_array[ray_num] = temp;
        ray_num++;
      } 
      for(int idx = 1; idx <= rays_down; idx++) {
        temp = new Ray(c_x1, c_y1 - idx*y_inc, c_x2, c_y2 - idx*y_inc, 0);
        input_array[ray_num] = temp;
        ray_num++;
      } 
    } else {    //greater than 90
      t = radians(180.0 - (float)t);   **
      double x_inc = spacing / sin((float)t);
      double y_inc = spacing / cos((float)t);
  
      double c_x1 = x_min;  **
      double c_y1 = y_max;
      double c_x2 = c_x1 + 15 * cos((float) t);  **
      double c_y2 = c_y1 - 15 * sin((float) t);
      int ray_num = 0;
      int rays_across = (int) (x_range / x_inc);
      int rays_down   = (int) (y_range / y_inc);
      
      for(int idx = 0; idx <= rays_across; idx++) {
        temp = new Ray(c_x1 + idx*x_inc, c_y1, c_x2 + idx*x_inc, c_y2, 0);  **
        input_array[ray_num] = temp;
        ray_num++;
      } 
      for(int idx = 1; idx <= rays_down; idx++) {
        temp = new Ray(c_x1, c_y1 - idx*y_inc, c_x2, c_y2 - idx*y_inc, 0);
        input_array[ray_num] = temp;
        ray_num++;
      } 
    }
    
  } 
  */
  
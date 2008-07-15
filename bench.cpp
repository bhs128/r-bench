#include <QtGui>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265359
#endif

#include "bench.h"

int X_Clicked,Y_Clicked;

Bench::Bench(QWidget *parent)
    : QWidget(parent)
{
	X_Clicked = Y_Clicked = 0;
}


void Bench::mousePressEvent(QMouseEvent *event)
{
	QPointF point = event->pos() - rect().center();
	X_Clicked = (int) point.x();
	Y_Clicked = (int) point.y();
    update();
}

void Bench::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    int side = qMin(width(), height());

    painter.setViewport((width() - side) / 2, (height() - side) / 2,
                        side, side);
    painter.setWindow(-50, -50, 100, 100);

    draw(&painter);
}

void Bench::draw(QPainter *painter)
{
    QColor niceYellow(255,230,104);
    QPen thinPen(niceYellow, 0.5);
	
    painter->setPen(thinPen);
    painter->drawLine(0,0,(X_Clicked*painter->window().width())/painter->viewport().width(), 
						  (Y_Clicked*painter->window().height())/painter->viewport().height() );


}

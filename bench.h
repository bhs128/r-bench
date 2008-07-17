#ifndef BENCH_H
#define BENCH_H

#include <QWidget>
#include <QLineF>
#include <QVector>
#include <QRect>

class Bench : public QWidget
{
    Q_OBJECT

public:
    Bench(QWidget *parent = 0);
   // int duration() const;
    void draw(QPainter *painter);

//signals:
   // void timeout();

public slots:
	void setTheta(int theta);
    void setRaySpacing(double s);
	
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
	void setLights(double theta);
   QRectF window;
   // QDateTime finishTime;
   // QTimer *updateTimer;
   // QTimer *finishTimer;
};

#endif

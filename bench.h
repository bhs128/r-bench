#ifndef BENCH_H
#define BENCH_H

#include <QWidget>

class Bench : public QWidget
{
    Q_OBJECT

public:
    Bench(QWidget *parent = 0);

   // void setDuration(int secs);
   // int duration() const;
    void draw(QPainter *painter);

signals:
   // void timeout();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
   // QDateTime finishTime;
   // QTimer *updateTimer;
   // QTimer *finishTimer;
};

#endif

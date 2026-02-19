/******************************************************************************
R-Bench - Reflector Workbench models how light reflects off mirrors of 
	various shapes
Copyright (C) 2008  Benjamin H. Schaffhausen

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
******************************************************************************/
#ifndef BENCH_H
#define BENCH_H

#include <QtWidgets>
#include <QWidget>
#include <QLineF>
#include <QVector>
#include <QRect>
#include <QTimer>
#include <QElapsedTimer>
#include <cmath>
#include <cstdint>
#include <thread>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265359
#endif

#include "reflector.h"
#include "receiver.h"

#define SCALER 1024
#define RAY_WIDTH 6.0

class Bench : public QWidget {
    Q_OBJECT

public:
    Bench(QWidget *parent = 0);
	
signals:
 	void alphaChanged(double a); 

	void hitsChanged(double a); 
	void radiusChanged(double r);
	
public slots:
	// Light Source Related
	void setTheta(int theta); // Change angle of incoming light
    void setRaySpacing(int s);
	//Reflector Related	
	void setParabola(bool);
	void setCatenary(bool);
	void setSemi(bool);
	void setReflectorMin(double);
	void setReflectorMax(double);
	void setAlpha(int alpha); // Change reflector  scaleing factor
	//Receiver Related	
	void setReceiverEnabled(int);
	void setSize(int);
	void setAngle(int);
	void setPanel(bool);
	void setPipe(bool);
	void setUnits(int);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
	void resizeEvent(QResizeEvent * event);

private:
	void setLights();
	void bounce(QLineF *a_ray, QVector<QLineF> &localFinal, int &localHits);	
	void drawFps(QPainter *painter);
	void renderFrame();
	void runSimulation();
	void calculateWindow();
	void worldToPixel(double wx, double wy, int &px, int &py) const;
	static void rasterLine(uint32_t *bits, int stride, int imgW, int imgH,
	                       int x0, int y0, int x1, int y1, uint32_t color);
	static void rasterFilledCircle(uint32_t *bits, int stride, int imgW, int imgH,
	                               int cx, int cy, int r, uint32_t fill, uint32_t stroke);
	void scheduleUpdate();
	double getWatts();
	
	QRect  window;
	Reflector mirror;
	Receiver sink;
	QVector<QLineF> InitialRays;
	QVector<QLineF> FinalRays;
	QImage frameBuffer;
	
	float w_right, w_left, w_top, w_bottom, w_width, w_height;
	float px_per_unit;
	float subunits_per_px;
	double watts_per_unit_2;
	bool  Receiver_Enabled;
	double 	RaySpacing;
	double	Theta; // In degrees
	QTimer updateTimer;
	bool lightsDirty;
	bool simDirty;

	// FPS tracking
	QElapsedTimer fpsTimer;
	QElapsedTimer perfTimer;
	int frameCount;
	double currentFps;
	double simMs;
	double paintMs;

	int numThreads;

};

#endif

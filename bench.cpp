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
#include "bench.h"

Bench::Bench(QWidget *parent) 
	: QWidget(parent), mirror(0.25, -2.0, 2.0), sink(0.0, 1.0, 0.25) 
	{
	w_right = 3.0 * SCALER;
	w_left = -3.0 * SCALER;
	subunits_per_px = (6.0 * SCALER) / 400.0;
	RaySpacing = .125  * SCALER;
	Receiver_Enabled = false;
	watts_per_unit_2  = 92.90304;
}

void Bench::setTheta(int t) {
	Theta = (double) t;
	setLights();
}
void Bench::setParabola(bool) {
	mirror.setShape(PARA);
	runSimulation();	
}

void Bench::setCatenary(bool) {
	mirror.setShape(CAT);
	runSimulation();
}

void Bench::setSemi(bool) {
	mirror.setShape(SEMI);
	runSimulation();
}

void Bench::setPanel(bool) {
	sink.setShape(LINE);
	runSimulation();
}

void Bench::setPipe(bool) {
	sink.setShape(CIRCLE);
	runSimulation();
}

void Bench::setRaySpacing(int s) {
	RaySpacing = (double) (s / 100.0) * SCALER;
	emit spacingChanged((double) (s / 100.0) );
	setLights();
}

void Bench::setReceiverEnabled(int state) {
	if(state == Qt::Checked) {
		Receiver_Enabled = true;
	} else {
		Receiver_Enabled = false;
		hitsChanged(0.0);
	}
	runSimulation();	
}
	
void Bench::setReflectorMin(double min) {
	mirror.setFmin(min);
	calculateWindow();
	runSimulation();
}

void Bench::setReflectorMax(double max) { // slider widget only uses int's - max is in hundredths
	mirror.setFmax(max);
	calculateWindow();
	runSimulation();
}
	
void Bench::setAlpha(int alpha) { // slider widget only uses int's - alpha is in hundredths
	mirror.setAlpha((double) alpha / 100.0);
	emit alphaChanged((double) alpha / 100.0);
	runSimulation();
}

void Bench::setSize(int r) {
	sink.setSize((double) r / 100.0);
	emit radiusChanged((double) r / 100.0);
	runSimulation();
}

void Bench::setAngle(int a) {
	sink.setAngle(a);
	runSimulation();
}
void Bench::setUnits(int u) {
	switch(u) {
		case 0:
			watts_per_unit_2 = 92.90304; //ft is default
			break;
		case 1:
			watts_per_unit_2 = 1000.0; //meters
			break;
		case 2:
			watts_per_unit_2 = 0.64516; //meters	
			break;
	}
	if(Receiver_Enabled) {
		emit hitsChanged( getWatts() );
	}
}

void Bench::mousePressEvent(QMouseEvent *event) {
	QPointF pos;
	
    pos = event->localPos();
	sink.setCenter(pos.x() * subunits_per_px + w_left, 
				   (pos.y() * subunits_per_px - w_top) * -1.0);
	runSimulation();
}

void Bench::resizeEvent ( QResizeEvent * event ) {
	calculateWindow();
}

void Bench::calculateWindow() {
	double reflector_width;

	reflector_width = mirror.fMax() - mirror.fMin();
	subunits_per_px = ( (100.0 * reflector_width / 70.0) * SCALER) / width();
	w_width = subunits_per_px * width();
	w_right = w_width*0.15 + mirror.fMax()* SCALER;
	w_left =  mirror.fMin()* SCALER - w_width*0.15;
	
	w_bottom = -0.10 * subunits_per_px * height();
	w_top = subunits_per_px * height() + w_bottom;
	w_height = w_top - w_bottom;
	
	window = QRect( (int) w_left, (int) (-1 * w_top), (int) w_width, (int) w_height);
	
	setLights();
}

void Bench::paintEvent(QPaintEvent *event) {
	QMatrix reflectionMatrix(1, 0, 0, -1, 0.0, 0.0); // Defines a reflection over the x-axis
    QPainter painter(this);
	painter.setMatrix(reflectionMatrix);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setWindow(window); // in case a resizeEvent updated it 
	
	drawGrid(&painter);	
    drawRays(&painter);
	mirror.draw(&painter);
	if(Receiver_Enabled) 
		sink.draw(&painter);
}

void Bench::drawRays(QPainter *painter) {
    QColor niceYellow(255,255,0, 128);
    QPen yellowPen(niceYellow);
    painter->setPen(yellowPen);

	//painter->drawLines(InitialRays);
	
    QColor niceOrange(255,128,0, 128);
    QPen orangePen(niceOrange);
    orangePen.setWidthF(6);
    painter->setPen(orangePen);
	
	painter->drawLines(FinalRays);
}

double Bench::getWatts() {
	double v = watts_per_unit_2 * ((double) sink.get_hits()) / (1.0 / (RaySpacing/SCALER));
	return ((int) (v * 10.0 )) / 10.0;
}

void Bench::drawGrid(QPainter *painter) {
	QColor darkGrey(64,64,64,128);
    QPen   gridPen(darkGrey);
	
	painter->fillRect( (int) w_left, (int) w_bottom, (int) w_width, (int) w_height, QColor(0,0,0)); 
	painter->setPen(gridPen);
	// Horizontal grid lines
	for(int y=0; y<w_top; y+=SCALER) {
		painter->drawLine( QLineF(w_left, (float) y, w_right, (float) y) );
		painter->drawLine( QLineF(w_left, (float) y * -1.0, w_right, (float) y * -1.0) );
	}
	// Vertical grid lines
	for(int x=0; x<w_right; x+=SCALER)
		painter->drawLine( QLineF((float) x, w_top, (float) x, w_bottom) );
		
	for(int x=0; x>w_left; x-=SCALER) 
		painter->drawLine( QLineF((float) x, w_top, (float) x, w_bottom) );

	
}

void Bench::setLights() {
	double theta = Theta;
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
		float c_x2 = c_x1 + (((theta < 90.0) ? -50 : 50 ) * SCALER*std::cos(t));
		float c_y2 = c_y1 - SCALER*50.0 * std::sin(t);
		
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
	runSimulation(); // Incoming rays have changed- need to re-run simulation
}

void Bench::runSimulation() {
	FinalRays.resize(0);  // reset array to null
	sink.reset_hits();
	mirror.reset_hits();
	for(int ridx = 0; ridx < InitialRays.size(); ridx++) {
		QLineF tmp(InitialRays[ridx]); // copy constructor
		bounce( &tmp ); 
			
		FinalRays.append(tmp);  // note- every input ray ends up in output array
	}
	if(Receiver_Enabled) {
		emit hitsChanged( getWatts() );
	}
	update(); // schedule a repaint of new rays
	
}

void Bench::bounce(QLineF *a_ray) {
	QLineF temp;
	QPointF pivot;
	QPointF pi, ri; // pipe intersection, & reflector intersection
	bool go_reflect = false;
	bool go_receive = false;
	
	if(mirror.intersects(a_ray)) {
		ri = mirror.intersection_coord(a_ray);
		go_reflect = true;
	}
	if(Receiver_Enabled && sink.intersects(a_ray)) {
		pi = sink.intersection_coord(a_ray);
		go_receive = true;
	}
	
	// does ray intersect both receiver & reflector?    
	if(go_receive && go_reflect) {      
		double pipe_dist = std::sqrt(std::pow(a_ray->x1() - pi.x(),2) + std::pow(a_ray->y1() - pi.y(), 2));
		double ref_dist  = std::sqrt(std::pow(a_ray->x1() - ri.x(),2) + std::pow(a_ray->y1() - ri.y(), 2));
		
		// - which one does it hit first?
		if(pipe_dist < ref_dist) {
			go_reflect = false; // hit pipe first, don't reflect it
		} else {
			go_receive = false; // hit reflctor first, don't absorb it
		}
	}
	
	if(go_receive) {
		// Ray stops at receiver
		a_ray->setP2(pi);
		sink.got_hit();
	}
	
	if(go_reflect) {
		temp = mirror.reflected_ray(a_ray);
		pivot = mirror.intersection_coord(a_ray);
		a_ray->setP2(pivot);
		temp.setP1(pivot);
		
		bounce(&temp);
	
		FinalRays.append(temp);
	}
}

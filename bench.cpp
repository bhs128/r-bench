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
#include <algorithm>
#include <cstdint>

// Cohen-Sutherland region outcodes
static inline int csOutcode(int x, int y, int w, int h) {
    int code = 0;
    if (x < 0) code |= 1;        // left
    else if (x >= w) code |= 2;   // right
    if (y < 0) code |= 4;        // above
    else if (y >= h) code |= 8;   // below
    return code;
}

Bench::Bench(QWidget *parent) 
	: QWidget(parent), mirror(0.25, -2.0, 2.0), sink(0.0, 1.0, 0.25) 
	{
	setAttribute(Qt::WA_OpaquePaintEvent);
	setMouseTracking(false); // only track when button is held (default)
	w_right = 3.0 * SCALER;
	w_left = -3.0 * SCALER;
	subunits_per_px = (6.0 * SCALER) / 400.0;
	RaySpacing = .125  * SCALER;
	Receiver_Enabled = false;
	watts_per_unit_2  = 92.90304;
	lightsDirty = false;
	simDirty = false;
	frameCount = 0;
	currentFps = 0.0;
	simMs = 0.0;
	paintMs = 0.0;
	numThreads = std::max(1, (int)std::thread::hardware_concurrency());
	fpsTimer.start();
	updateTimer.setSingleShot(true);
	updateTimer.setInterval(16); // ~60fps cap
	connect(&updateTimer, &QTimer::timeout, this, [this]() {
		if (lightsDirty) {
			lightsDirty = false;
			setLights(); // setLights calls runSimulation
		} else if (simDirty) {
			simDirty = false;
			runSimulation();
		}
	});
}

void Bench::setTheta(int t) {
	Theta = (double) t;
	lightsDirty = true;
	scheduleUpdate();
}
void Bench::setParabola(bool) {
	mirror.setShape(PARA);
	simDirty = true;
	scheduleUpdate();
}

void Bench::setCatenary(bool) {
	mirror.setShape(CAT);
	simDirty = true;
	scheduleUpdate();
}

void Bench::setSemi(bool) {
	mirror.setShape(SEMI);
	simDirty = true;
	scheduleUpdate();
}

void Bench::setPanel(bool) {
	sink.setShape(LINE);
	simDirty = true;
	scheduleUpdate();
}

void Bench::setPipe(bool) {
	sink.setShape(CIRCLE);
	simDirty = true;
	scheduleUpdate();
}

void Bench::setRaySpacing(int s) {
	RaySpacing = s * RAY_WIDTH;
	lightsDirty = true;
	scheduleUpdate();
}

void Bench::setReceiverEnabled(int state) {
	if(state == Qt::Checked) {
		Receiver_Enabled = true;
	} else {
		Receiver_Enabled = false;
		hitsChanged(0.0);
	}
	simDirty = true;
	scheduleUpdate();
}
	
void Bench::setReflectorMin(double min) {
	mirror.setFmin(min);
	calculateWindow();
}

void Bench::setReflectorMax(double max) {
	mirror.setFmax(max);
	calculateWindow();
}
	
void Bench::setAlpha(int alpha) {
	mirror.setAlpha((double) alpha / 100.0);
	emit alphaChanged((double) alpha / 100.0);
	simDirty = true;
	scheduleUpdate();
}

void Bench::setSize(int r) {
	sink.setSize((double) r / 100.0);
	emit radiusChanged((double) r / 100.0);
	simDirty = true;
	scheduleUpdate();
}

void Bench::setAngle(int a) {
	sink.setAngle(a);
	simDirty = true;
	scheduleUpdate();
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
			watts_per_unit_2 = 0.64516; //inches
			break;
	}
	if(Receiver_Enabled) {
		emit hitsChanged( getWatts() );
	}
}

void Bench::mousePressEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		QPointF pos = event->position();
		sink.setCenter(pos.x() * subunits_per_px + w_left, 
					   (pos.y() * subunits_per_px - w_top) * -1.0);
		simDirty = true;
		scheduleUpdate();
	}
}

void Bench::mouseMoveEvent(QMouseEvent *event) {
	if (event->buttons() & Qt::LeftButton) {
		QPointF pos = event->position();
		sink.setCenter(pos.x() * subunits_per_px + w_left, 
					   (pos.y() * subunits_per_px - w_top) * -1.0);
		simDirty = true;
		scheduleUpdate();
	}
}

void Bench::resizeEvent ( QResizeEvent * ) {
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
	
	lightsDirty = true;
	scheduleUpdate();
}

void Bench::scheduleUpdate() {
	if (!updateTimer.isActive()) {
		updateTimer.start();
	}
}

void Bench::paintEvent(QPaintEvent *) {
	// FPS tracking
	frameCount++;
	qint64 elapsed = fpsTimer.elapsed();
	if (elapsed >= 500) {
		currentFps = frameCount * 1000.0 / elapsed;
		frameCount = 0;
		fpsTimer.restart();
	}

	QPainter painter(this);
	if (!frameBuffer.isNull()) {
		painter.drawImage(0, 0, frameBuffer);
	} else {
		painter.fillRect(rect(), Qt::black);
	}
	drawFps(&painter);
}

void Bench::drawFps(QPainter *painter) {
	painter->resetTransform();
	painter->setWindow(rect());
	QFont font;
	font.setPixelSize(12);
	painter->setFont(font);
	painter->setPen(QColor(0, 255, 0));
	QString fpsText = QString("%1 fps | %2 rays | sim %3ms | paint %4ms")
		.arg(currentFps, 0, 'f', 1)
		.arg(FinalRays.size())
		.arg(simMs, 0, 'f', 1)
		.arg(paintMs, 0, 'f', 1);
	painter->drawText(6, 16, fpsText);
}

void Bench::worldToPixel(double wx, double wy, int &px, int &py) const {
	px = (int)((wx - w_left) / w_width * frameBuffer.width());
	py = (int)((w_top - wy) / w_height * frameBuffer.height());
}

double Bench::getWatts() {
	double v = watts_per_unit_2 * ((double) sink.get_hits()) / (1.0 / (RaySpacing/SCALER));
	return ((int) (v * 10.0 )) / 10.0;
}

void Bench::rasterLine(uint32_t *bits, int stride, int imgW, int imgH,
                        int x0, int y0, int x1, int y1, uint32_t color) {
	// Cohen-Sutherland line clipping
	int code0 = csOutcode(x0, y0, imgW, imgH);
	int code1 = csOutcode(x1, y1, imgW, imgH);

	for (;;) {
		if (!(code0 | code1)) break;   // both inside
		if (code0 & code1) return;     // trivial reject

		int code = code0 ? code0 : code1;
		int x, y;

		if (code & 8) {        // below
			x = x0 + (int)((long long)(x1 - x0) * (imgH - 1 - y0) / (y1 - y0));
			y = imgH - 1;
		} else if (code & 4) { // above
			x = x0 + (int)((long long)(x1 - x0) * (0 - y0) / (y1 - y0));
			y = 0;
		} else if (code & 2) { // right
			y = y0 + (int)((long long)(y1 - y0) * (imgW - 1 - x0) / (x1 - x0));
			x = imgW - 1;
		} else {               // left
			y = y0 + (int)((long long)(y1 - y0) * (0 - x0) / (x1 - x0));
			x = 0;
		}

		if (code == code0) {
			x0 = x; y0 = y;
			code0 = csOutcode(x0, y0, imgW, imgH);
		} else {
			x1 = x; y1 = y;
			code1 = csOutcode(x1, y1, imgW, imgH);
		}
	}

	// Bresenham's line algorithm
	int dx = std::abs(x1 - x0);
	int dy = -std::abs(y1 - y0);
	int sx = x0 < x1 ? 1 : -1;
	int sy = y0 < y1 ? 1 : -1;
	int err = dx + dy;

	for (;;) {
		bits[y0 * stride + x0] = color;
		if (x0 == x1 && y0 == y1) break;
		int e2 = 2 * err;
		if (e2 >= dy) { err += dy; x0 += sx; }
		if (e2 <= dx) { err += dx; y0 += sy; }
	}
}

void Bench::rasterFilledCircle(uint32_t *bits, int stride, int imgW, int imgH,
                                int cx, int cy, int r, uint32_t fill, uint32_t stroke) {
	int r2 = r * r;
	for (int dy = -r; dy <= r; dy++) {
		int y = cy + dy;
		if (y < 0 || y >= imgH) continue;
		int dx = (int)std::sqrt((double)(r2 - dy * dy));
		int xLeft = cx - dx;
		int xRight = cx + dx;
		int xStart = std::max(0, xLeft);
		int xEnd = std::min(imgW - 1, xRight);
		uint32_t *row = bits + y * stride;
		for (int x = xStart; x <= xEnd; x++) {
			row[x] = fill;
		}
		// Stroke boundary pixels
		if (xLeft >= 0 && xLeft < imgW) row[xLeft] = stroke;
		if (xRight >= 0 && xRight < imgW) row[xRight] = stroke;
	}
}

void Bench::renderFrame() {
	int imgW = width();
	int imgH = height();
	if (imgW <= 0 || imgH <= 0) return;

	if (frameBuffer.width() != imgW || frameBuffer.height() != imgH) {
		frameBuffer = QImage(imgW, imgH, QImage::Format_RGB32);
	}

	frameBuffer.fill(0xFF000000);

	uint32_t *bits = reinterpret_cast<uint32_t *>(frameBuffer.bits());
	int stride = frameBuffer.bytesPerLine() / 4;

	// ---- Grid ----
	uint32_t gridColor = 0xFF404040;
	for (int y = 0; y < (int)w_top; y += SCALER) {
		int px0, py0, px1, py1;
		worldToPixel(w_left, (double)y, px0, py0);
		worldToPixel(w_right, (double)y, px1, py1);
		rasterLine(bits, stride, imgW, imgH, px0, py0, px1, py1, gridColor);
		worldToPixel(w_left, (double)(-y), px0, py0);
		worldToPixel(w_right, (double)(-y), px1, py1);
		rasterLine(bits, stride, imgW, imgH, px0, py0, px1, py1, gridColor);
	}
	for (int x = 0; x < (int)w_right; x += SCALER) {
		int px0, py0, px1, py1;
		worldToPixel((double)x, w_top, px0, py0);
		worldToPixel((double)x, w_bottom, px1, py1);
		rasterLine(bits, stride, imgW, imgH, px0, py0, px1, py1, gridColor);
	}
	for (int x = 0; x > (int)w_left; x -= SCALER) {
		int px0, py0, px1, py1;
		worldToPixel((double)x, w_top, px0, py0);
		worldToPixel((double)x, w_bottom, px1, py1);
		rasterLine(bits, stride, imgW, imgH, px0, py0, px1, py1, gridColor);
	}

	// ---- Rays (multi-threaded) ----
	uint32_t rayColor = 0xFFFF8000;
	const int numRays = FinalRays.size();
	if (numRays > 0) {
		const int chunkSize = (numRays + numThreads - 1) / numThreads;
		std::vector<std::thread> threads;
		for (int t = 0; t < numThreads; ++t) {
			int start = t * chunkSize;
			int end = std::min(start + chunkSize, numRays);
			if (start >= end) break;
			threads.emplace_back([this, bits, stride, imgW, imgH, rayColor, start, end]() {
				for (int i = start; i < end; ++i) {
					const QLineF &ray = FinalRays[i];
					int px0, py0, px1, py1;
					worldToPixel(ray.x1(), ray.y1(), px0, py0);
					worldToPixel(ray.x2(), ray.y2(), px1, py1);
					rasterLine(bits, stride, imgW, imgH, px0, py0, px1, py1, rayColor);
				}
			});
		}
		for (auto &th : threads) th.join();
	}

	// ---- Reflector ----
	uint32_t refColor = 0xFFC4C4C4;
	const auto &segs = mirror.getSegments();
	for (const auto &seg : segs) {
		int px0, py0, px1, py1;
		worldToPixel(seg.x1(), seg.y1(), px0, py0);
		worldToPixel(seg.x2(), seg.y2(), px1, py1);
		rasterLine(bits, stride, imgW, imgH, px0, py0, px1, py1, refColor);
	}

	// ---- Receiver ----
	if (Receiver_Enabled) {
		if (sink.getShape() == CIRCLE) {
			int cx, cy;
			worldToPixel(sink.getCenterXRaw(), sink.getCenterYRaw(), cx, cy);
			int rx, dummy;
			worldToPixel(sink.getCenterXRaw() + sink.getSizeRaw(), sink.getCenterYRaw(), rx, dummy);
			int r = std::abs(rx - cx);
			if (r > 0)
				rasterFilledCircle(bits, stride, imgW, imgH, cx, cy, r, 0xFF333366, 0xFFFFFFFF);
		} else { // LINE
			const QLineF &panel = sink.getPanel();
			int px0, py0, px1, py1;
			worldToPixel(panel.x1(), panel.y1(), px0, py0);
			worldToPixel(panel.x2(), panel.y2(), px1, py1);
			rasterLine(bits, stride, imgW, imgH, px0, py0, px1, py1, 0xFFFFFFFF);
		}
	}
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
	perfTimer.start();
	const int numRays = InitialRays.size();
	const int chunkSize = (numRays + numThreads - 1) / numThreads;

	// Per-thread output buffers
	std::vector<QVector<QLineF>> threadPrimary(numThreads);
	std::vector<QVector<QLineF>> threadBounced(numThreads);
	std::vector<int> threadHits(numThreads, 0);

	std::vector<std::thread> threads;
	for (int t = 0; t < numThreads; ++t) {
		int start = t * chunkSize;
		int end = std::min(start + chunkSize, numRays);
		if (start >= end) break;
		threads.emplace_back([&, t, start, end]() {
			int localHits = 0;
			for (int ridx = start; ridx < end; ++ridx) {
				QLineF tmp(InitialRays[ridx]);
				bounce(&tmp, threadBounced[t], localHits);
				threadPrimary[t].append(tmp);
			}
			threadHits[t] = localHits;
		});
	}
	for (auto &th : threads) th.join();

	// Merge results
	FinalRays.resize(0);
	sink.reset_hits();
	mirror.reset_hits();
	int totalHits = 0;
	for (int t = 0; t < (int)threads.size(); ++t) {
		FinalRays.append(threadPrimary[t]);
		FinalRays.append(threadBounced[t]);
		totalHits += threadHits[t];
	}
	sink.set_hits(totalHits);
	simMs = perfTimer.nsecsElapsed() / 1e6;

	perfTimer.start();
	renderFrame();
	paintMs = perfTimer.nsecsElapsed() / 1e6;

	if(Receiver_Enabled) {
		emit hitsChanged( getWatts() );
	}
	update();
}

void Bench::bounce(QLineF *a_ray, QVector<QLineF> &localFinal, int &localHits) {
	QLineF temp;
	QPointF pi, ri; // pipe intersection, & reflector intersection
	bool go_reflect = false;
	bool go_receive = false;
	
	go_reflect = mirror.findIntersection(a_ray, ri);
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
		localHits++;
	}
	
	if(go_reflect) {
		temp = mirror.reflected_ray(a_ray, ri);
		a_ray->setP2(ri);
		temp.setP1(ri);
		
		bounce(&temp, localFinal, localHits);
	
		localFinal.append(temp);
	}
}

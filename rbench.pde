PFont gill_16;
int MAX_RAYS = 1000;
double RAY_SPACING = 0.0625;
float[] days_data = new float[180];
Reflector cat;
Bench b;
double theta;
double cur_multi;
double total_multi;
double day_avg;

int x_min_px = 40;
int x_max_px = 600;
int y_min_px = 410;
int y_max_px = 60;
double f_min = -2.5;
double f_max = 2.5;
double zoom = 100.0;

// flags
boolean animate = true;
boolean dragging_pipe = false;

void setup() {
  size(640, 480);
  smooth();
  gill_16 = loadFont("GillSansMT-16.vlw");
    
  int center_px = (x_max_px - x_min_px)/2 + x_min_px;
  double func_center = (f_max - f_min)/2.0 + f_min;
  
   // create Bench - Origin placed so that the screen is centered on the reflector
  b = new Bench(zoom, x_min_px, y_max_px, 
                x_max_px, y_min_px, 
                center_px + (int) (func_center * zoom), 390);
  b.add_reflector(.25, f_min, f_max); // "place the mirror"
  b.add_receiver(0.0, 1.5, 0.125); // add receiver
  theta = 5.0;
  cur_multi = total_multi = day_avg = 0.0;
}

void mousePressed() {
    if(dist(map_x(0.0), map_y(1.5), mouseX, mouseY) <= (0.125 * zoom)) {
      dragging_pipe = true;
    } else {
      dragging_pipe = false;
    }
}
void mouseReleased() { dragging_pipe = false; }

void mouseMoved() { 
  if(dragging_pipe) {
    b.add_receiver(map(mouseX, x_min_px, x_max_px, (float) b.get_xmin(), (float) b.get_xmax()),
                   map(mouseY, y_min_px, y_max_px, (float) b.get_ymin(), (float) b.get_ymax()),
                   0.125);
  }
}

void draw() { 
  b.add_directional_light(theta, RAY_SPACING); // "set up the light"

  b.go(); // do the reflecting eg "turn on the lights"
  
  // <-- analyse the Bench here, after the calculating has been done.

  b.draw(); // What does it look like?
  
  draw_frame(); // hide the overflow rays

  // draw GUI elements here, on top of the frame.
  cur_multi = (float) b.receiver_hits() / (.125 * 2.0 / RAY_SPACING);
  total_multi += cur_multi;
  fill(0);
  textFont(gill_16);
  text( "Multiplier: " + round((float)cur_multi * 10.0) / 10.0, 40, 430);
  text( "Average: " + round((float) (total_multi/(theta-4.0)) * 10.0) / 10.0, 40, 450); 
  text( "Daily Average: " + round((float) day_avg * 10.0) / 10.0, 40, 470);
  days_data[(int) theta] = (float) cur_multi;
  
  for(int d_idx = 0; d_idx < 180; d_idx++) {
    stroke(0);
    fill(0);
    if(days_data[d_idx] > 0.0) 
      ellipse(map(d_idx, 0, 180, x_min_px, x_max_px), map(days_data[d_idx], 0, 20, y_max_px-2, 2), 1.0 , 1.0);
  }
  stroke(0,0,255);
  line(map((float) theta, 0, 180, x_min_px, x_max_px), 2, map((float)theta, 0, 180, x_min_px, x_max_px), y_max_px-2);

  if(theta > 175.0) { // animate from 5 to 175 degrees and back again
    day_avg = total_multi/(theta-4.0);
    theta = 5.0;
    total_multi = 0.0;
  } else {
    theta += 1.0;
  }
}

void draw_frame() { // This masks the area around the Bench/graph
  fill(240);
  stroke(240);
    
  rectMode(CORNERS);
  rect(0, 0,               width, y_max_px);    // header
  rect(0, y_max_px,        x_min_px, y_min_px); // left side
  rect(x_max_px, y_max_px, width, y_min_px);    // right side
  rect(0, y_min_px,        width, height);      // footer
    
}
  
float map_x(double x) { 
  return map((float) x, 
           (float) b.get_xmin(), (float) b.get_xmax(),
           x_min_px, x_max_px);
}

float map_y(double y) {
  return map((float) y,
           (float) b.get_ymin(), (float) b.get_ymax(), 
           y_min_px, y_max_px);
}

PFont fontA;
int MAX_RAYS = 1000;
double RAY_SPACING = 0.0625;
Reflector cat;
Bench b;
double theta;
double cur_multi;
double total_multi;
double day_avg;

int x_min_px = 40;
int x_max_px = 600;
int y_min_px = 410;
int y_max_px = 20;
double f_min = -2.5;
double f_max = 2.5;
double zoom = 100.0;

void setup() {
  size(640, 480);
  smooth();
  fontA = loadFont("Ziggurat-HTF-Black-32.vlw");
  textFont(fontA, 16);
  
  int center_px = (x_max_px - x_min_px)/2 + x_min_px;
  double func_center = (f_max - f_min)/2.0 + f_min;
  
   // create Bench - Origin placed so that the screen is centered on the reflector
  b = new Bench(zoom, x_min_px, y_max_px, 
                x_max_px, y_min_px, 
                center_px + (int) (func_center * zoom), 390);
  theta = 5.0;
  cur_multi = total_multi = day_avg = 0.0;
}

void draw() { 
  b.add_directional_light(theta, 0.06125); // "set up the light"
  b.add_reflector(.5, f_min, f_max); // "place the mirror"
  b.add_receiver(0.0, 0.75, 0.125) ; // add receiver
  b.go(); // do the reflecting eg "turn on the lights"
  
  // <-- analyse the Bench here, after the calculating has been done.

  b.draw(); // What does it look like?
  
  draw_frame(); // hide the overflow rays
  cur_multi = (float) b.receiver_hits() / (.125 * 2.0 / .06125);
  total_multi += cur_multi;
  fill(0);
  text( "Multiplier: " + cur_multi, 40, 430);
  text( "Average: " + total_multi/(theta-4.0), 40, 450); 
  text( "Daily Average: " + day_avg, 40, 470);
  // <-- draw GUI elements here, on top of the frame.
 
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

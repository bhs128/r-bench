int MAX_RAYS = 1000;
double RAY_SPACING = 0.0625;
Reflector cat;
Bench b;
double theta;

int x_min_px = 40;
int x_max_px = 550;
int y_min_px = 410;
int y_max_px = 20;

void setup() {
  size(640, 480);
  smooth();
  
  b = new Bench(75.0, x_min_px, y_max_px, x_max_px, y_min_px, (x_max_px - x_min_px)/2, 390);
  theta = 5.0;
}

void draw() { 
  b.add_directional_light(theta, 0.06125); // "set up the light"
  b.add_reflector(.4, -2.5, 2.5); // "place the mirror"
  b.go(); // do the reflecting eg "turn on the lights"
  
  // <-- analyse the Bench here, after the calculating has been done.
  
  b.draw(); // What does it look like?
  
  draw_frame(); // hide the overflow rays
  
  // <-- draw GUI elements here, on top of the frame.
 
  if(theta > 175.0) { // animate from 5 to 175 degrees and back again
    theta = 5.0;
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

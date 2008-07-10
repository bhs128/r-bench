int MAX_RAYS = 1000;
double RAY_SPACING = 0.0625;
Reflector cat;
Bench b;
double theta;

int x_min_px = 10;
int x_max_px = 550;
int y_min_px = 410;
int y_max_px = 10;

void setup() {
  size(640, 480);
  smooth();
  
  b = new Bench(50.0, x_min_px, y_max_px, x_max_px, y_min_px, (x_max_px - x_min_px)/2, 390);
  theta = 5.0;
  
  //cat = new Reflector(.5, -2.5, 2.5);
}

void draw() {
  b.draw();
  b.add_directional_light(theta, 0.125);
  
  draw_frame();
  
  if(theta > 175.0) { // animate from 5 to 175 degrees and back again
    theta = 5.0;
  } else {
    theta += 1.0;
  }
  /*
  fill_sun_array(theta, RAY_SPACING);
  draw_graphpaper();
  Ray temp, temp2, temp3;
  Coord pivot, pivot2, pivot3;

for(int ridx = 0; ridx < 500; ridx++) {
//    int ridx = 24;
    if(sun_array[ridx] != null) {
      bounce(sun_array[ridx]);
      
      sun_array[ridx].draw(0);
    }
    bounce_num = 0;
  }
  strokeWeight(2);  
  cat.draw();
  strokeWeight(1);
  //theta = map(mouseX, 0.0, width, 175.0, 5.0); 
  //saveFrame("frame-####.png"); 
  */
  
}
  void draw_frame() {
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

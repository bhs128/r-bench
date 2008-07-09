int MAX_RAYS = 1000;
double RAY_SPACING = 0.0625;
Reflector cat;
double x_range = 6.0;
double y_range;
Ray[] sun_array = new Ray[MAX_RAYS];
float theta;
int bounce_num = 0;


void setup() {
  size(640, 480);
  smooth();
  y_range = x_range *  height / width;
  theta = 5.0;

  cat = new Reflector(.5, -2.5, 2.5);
}

void draw() {
  if(theta > 175.0) { // animate from 5 to 175 degrees and back again
    theta = 5.0;
  } else {
    theta += 1.0;
  }
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
}

Ray bounce(Ray a_ray) {
  Ray temp;
  Coord pivot;

  if(cat.intersects(a_ray)) {
    temp = cat.reflected_ray(a_ray);
    pivot = cat.intersection_coord(a_ray);
    a_ray.set_x2(pivot.getX());
    a_ray.set_y2(pivot.getY());
    temp.set_x1(pivot.getX());
    temp.set_y1(pivot.getY());
    
    bounce(temp);
   
    temp.draw(1); 
    return temp;
  }
  return new Ray(0.0,0.0,0.0,0.0);
}

void draw_graphpaper() {
  background(255);
  stroke(51,153,255);
  // Draw x-axis
  line(0.0, map_y(0.0), width, map_y(0.0));
  // Draw y-axis
  line(map_x(0.0), 0.0, map_x(0.0), height);    
}

void fill_sun_array(double t, double spacing) {
  float px_spacing = (float)(width*spacing/x_range);
  float num_rays = width / px_spacing;
  Ray temp;
  double len = sqrt(pow((float) x_range, 2) + pow((float) y_range, 2))*2.0;
  
  for(int idx = 0; idx < MAX_RAYS; idx++) { // reset array to null
    sun_array[idx] = null;
  }
  
  if(t == 90.0) {
    for(int idx = 0; idx < num_rays; idx++) {
      temp = new Ray(idx*spacing-x_range/2.0, y_range, idx*spacing-x_range/2.0, -1*y_range);
      //temp.draw();
      sun_array[idx] = temp;
    }
  } else if(t < 90.0) {
    t = radians((float)t);
    double x_inc = spacing / sin((float)t);
    double y_inc = spacing / cos((float)t);

    double c_x1 = x_range / 2.0;
    double c_y1 = (y_range/10.0)*9.0;
    double c_x2 = c_x1 - 15 * cos((float) t);
    double c_y2 = c_y1 - 15 * sin((float) t);
    int ray_num = 0;
    int rays_across = (int) (x_range / x_inc);
    int rays_down   = (int) (y_range / y_inc);
    
    for(int idx = 0; idx <= rays_across; idx++) {
      temp = new Ray(c_x1 - idx*x_inc, c_y1, c_x2 - idx*x_inc, c_y2);
      sun_array[ray_num] = temp;
      ray_num++;
    } 
    for(int idx = 1; idx <= rays_down; idx++) {
      temp = new Ray(c_x1, c_y1 - idx*y_inc, c_x2, c_y2 - idx*y_inc);
      sun_array[ray_num] = temp;
      ray_num++;
    } 
  } else {    //greater than 90
    t = radians(180.0 - (float)t);
    double x_inc = spacing / sin((float)t);
    double y_inc = spacing / cos((float)t);

    double c_x1 = x_range / -2.0;//**
    double c_y1 = (y_range/10.0)*9.0;
    double c_x2 = c_x1 + 15 * cos((float) t);//**
    double c_y2 = c_y1 - 15 * sin((float) t);
    int ray_num = 0;
    int rays_across = (int) (x_range / x_inc);
    int rays_down   = (int) (y_range / y_inc);
    
    for(int idx = 0; idx <= rays_across; idx++) {
      temp = new Ray(c_x1 + idx*x_inc, c_y1, c_x2 + idx*x_inc, c_y2);//**
      sun_array[ray_num] = temp;
      ray_num++;
    } 
    for(int idx = 1; idx <= rays_down; idx++) {
      temp = new Ray(c_x1, c_y1 - idx*y_inc, c_x2, c_y2 - idx*y_inc);
      sun_array[ray_num] = temp;
      ray_num++;
    } 
  }
}

float map_x(double x) {
  return map((float) x, 
             (float) x_range/2.0*-1, (float) x_range/2.0,
             0, width);
}
float map_y(double y) {
  return map((float) y,
             (float) y_range/10.0*-1, (float) y_range/10.0*9.0, 
             height, 0);
}  

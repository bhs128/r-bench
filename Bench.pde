class Bench {
  double x_min, x_max, y_min, y_max;
  double x_range, y_range;
  int x_min_px, x_max_px, y_min_px, y_max_px; // in pixels
  
  Bench(double px_per_unit, 
        int ul_x_px, int ul_y_px, 
        int lr_x_px, int lr_y_px, 
        int origin_x_px, int origin_y_px) {
    x_min_px = ul_x_px;
    x_max_px = lr_x_px;
    y_min_px = lr_y_px;
    y_max_px = ul_y_px;
    
    x_range = (double) (lr_x_px - ul_x_px) / px_per_unit;
    x_min = (double) (origin_x_px - lr_x_px) / px_per_unit;  
    x_max = x_min + x_range;

    y_range = (double) (lr_y_px - ul_y_px) / px_per_unit;      
    y_min = (double) (origin_y_px - lr_y_px) / px_per_unit;
    y_max = y_min + y_range;
  } 
  void draw() {
    draw_graphpaper();  
  }
  
  void draw_graphpaper() {
    fill(255);
    stroke(128);
    
    rectMode(CORNERS);
    rect(x_min_px, y_max_px, x_max_px, y_min_px);
    stroke(51,153,255);
    // Draw x-axis
    line(map_x(x_min), map_y(0.0), map_x(x_max), map_y(0.0));
    // Draw y-axis 
    line(map_x(0.0), map_y(y_max), map_x(0.0), map_y(y_min) );    
  }

Ray bounce(Ray a_ray) {
  /*
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
  */
  return new Ray(0.0,0.0,0.0,0.0);
}

void fill_sun_array(double t, double spacing) {
  /*
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
  */
}  
  double get_xmin() { return x_min; }
  double get_xmax() { return x_max; }
  double get_ymin() { return y_min; }
  double get_ymax() { return y_max; }
}

class Bench {
  double x_min, x_max, y_min, y_max;
  double x_range, y_range;
  int x_min_px, x_max_px, y_min_px, y_max_px, w_px, h_px; // in pixels
  Ray[] input_array  = new Ray[MAX_RAYS]; // incoming rays
  Ray[] output_array = new Ray[MAX_RAYS]; // final reflected rays
  int rb_idx = 0;
  Reflector r;
  
  Bench(double px_per_unit, 
        int ul_x_px, int ul_y_px, 
        int lr_x_px, int lr_y_px, 
        int origin_x_px, int origin_y_px) {
          
    // Pixel bounds of the Bench (where it should be drawn
    x_min_px = ul_x_px;
    x_max_px = lr_x_px;
    y_min_px = lr_y_px;
    y_max_px = ul_y_px;
    w_px = lr_x_px - ul_x_px;
    h_px = lr_y_px - ul_y_px;
    
    // Unit values for domain and range of values inside the Bench (what it represents)
    x_range = (double) (lr_x_px - ul_x_px) / px_per_unit;
    x_min = (double) (origin_x_px - lr_x_px) / px_per_unit;  
    x_max = x_min + x_range;

    y_range = (double) (lr_y_px - ul_y_px) / px_per_unit;      
    y_min = (double) (origin_y_px - lr_y_px) / px_per_unit;
    y_max = y_min + y_range;
  } 
  
  void draw() {
    draw_graphpaper();  
    draw_rays();
    draw_reflector();
  }
  
  void draw_graphpaper() {
    fill(255);
    stroke(128);
    
    rectMode(CORNERS);
    rect(x_min_px-1, y_max_px-1, x_max_px+1, y_min_px+1);
    stroke(51,153,255);
    // Draw x-axis
    line(map_x(x_min), map_y(0.0), map_x(x_max), map_y(0.0));
    // Draw y-axis 
    line(map_x(0.0), map_y(y_max), map_x(0.0), map_y(y_min) );    
  }

  void draw_rays() {
    for(int ridx = 0; ridx < MAX_RAYS; ridx++) {
      if(output_array[ridx] != null) {
        output_array[ridx].draw();
      }
    }
  }
  
  void draw_reflector() {
    if(r != null) {
      r.draw();
    }
  }
  
  void go() {
    for(int idx = 0; idx < MAX_RAYS; idx++) { // reset output array to null
      output_array[idx] = null;
    }
    rb_idx = 0; // Rays Bounced Index is now zero again
    
    for(int ridx = 0; ridx < MAX_RAYS; ridx++) {
      if(input_array[ridx] != null) {
        Ray tmp = input_array[ridx].copy();
        bounce( tmp );
      
        output_array[rb_idx] = tmp;
        rb_idx++;
      }
    }
  }
  
  void bounce(Ray a_ray) {
    Ray temp;
    Coord pivot;

    if(r.intersects(a_ray)) {
      temp = r.reflected_ray(a_ray);
      pivot = r.intersection_coord(a_ray);
      a_ray.set_x2(pivot.getX());
      a_ray.set_y2(pivot.getY());
      temp.set_x1(pivot.getX());
      temp.set_y1(pivot.getY());
    
      bounce(temp);
   
      output_array[rb_idx] = temp;
      rb_idx++;
    }
  }

  void add_directional_light(double t, double spacing) {
    float px_spacing = (float)(w_px*spacing/x_range);
    float num_rays = w_px / px_spacing;
    Ray temp;
    
    for(int idx = 0; idx < MAX_RAYS; idx++) { // reset array to null
      input_array[idx] = null;
    }
  
    if(t == 90.0) {
      for(int idx = 0; idx < num_rays; idx++) {
        temp = new Ray(idx*spacing+x_min, y_max, idx*spacing+x_min, y_min, 0);
        //temp.draw();
        input_array[idx] = temp;
      }
    } else if(t < 90.0) {
      t = radians((float)t);
      double x_inc = spacing / sin((float)t);
      double y_inc = spacing / cos((float)t);
  
      double c_x1 = x_max;
      double c_y1 = y_max;
      double c_x2 = c_x1 - 15 * cos((float) t);
      double c_y2 = c_y1 - 15 * sin((float) t);
      int ray_num = 0;
      int rays_across = (int) (x_range / x_inc);
      int rays_down   = (int) (y_range / y_inc);
      
      for(int idx = 0; idx <= rays_across; idx++) {
        temp = new Ray(c_x1 - idx*x_inc, c_y1, c_x2 - idx*x_inc, c_y2, 0);
        input_array[ray_num] = temp;
        ray_num++;
      } 
      for(int idx = 1; idx <= rays_down; idx++) {
        temp = new Ray(c_x1, c_y1 - idx*y_inc, c_x2, c_y2 - idx*y_inc, 0);
        input_array[ray_num] = temp;
        ray_num++;
      } 
    } else {    //greater than 90
      t = radians(180.0 - (float)t);
      double x_inc = spacing / sin((float)t);
      double y_inc = spacing / cos((float)t);
  
      double c_x1 = x_min;//**
      double c_y1 = y_max;
      double c_x2 = c_x1 + 15 * cos((float) t);//**
      double c_y2 = c_y1 - 15 * sin((float) t);
      int ray_num = 0;
      int rays_across = (int) (x_range / x_inc);
      int rays_down   = (int) (y_range / y_inc);
      
      for(int idx = 0; idx <= rays_across; idx++) {
        temp = new Ray(c_x1 + idx*x_inc, c_y1, c_x2 + idx*x_inc, c_y2, 0);//**
        input_array[ray_num] = temp;
        ray_num++;
      } 
      for(int idx = 1; idx <= rays_down; idx++) {
        temp = new Ray(c_x1, c_y1 - idx*y_inc, c_x2, c_y2 - idx*y_inc, 0);
        input_array[ray_num] = temp;
        ray_num++;
      } 
    }
    
  }  
  
  void add_reflector(double a, double f_min, double f_max) {
    r = new Reflector(a, f_min, f_max);
  }
  
  double get_xmin() { return x_min; }
  double get_xmax() { return x_max; }
  double get_ymin() { return y_min; }
  double get_ymax() { return y_max; }
}

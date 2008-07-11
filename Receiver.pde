class Receiver {
  double center_x, center_y, radius;
  int hits; 
  
  Receiver(double x, double y, double r) {
    center_x = x;
    center_y = y;
    radius = r;
    
    hits = 0;
  }
  void reset_hits() {
    hits = 0;  
  }
  
  void got_hit() {
    hits++;
  }
  
  int get_hits() {
    return hits;
  }
  
  boolean intersects(Ray a_ray) {
    double x1, y1, x2, y2; 
    x1 = a_ray.get_x1();
    y1 = a_ray.get_y1();
    x2 = a_ray.get_x2();
    y2 = a_ray.get_y2();
    
    if(a_ray.is_vertical()) {
      if(x1 < center_x - radius) {
        return false;
      }
      if(x1 > center_x + radius) {
        return false;
      }   
      if( y1 > center_y + radius && y2 < center_y - radius ) { // does it start above and end below?
        return true; 
      } else {
        return false;
      } 
    }
    // easy outs
    if((x1 < x2) && (center_x < x1 - radius)) 
      return false;
    
    if((x1 > x2) && (center_x > x1 + radius)) 
      return false;
        
    if((y1 < y2) && (center_y < y1 - radius)) 
      return false;
    
    if((y1 > y2) && (center_x > y1 + radius)) 
      return false;
      
    // translate points so the circle is at the origin
    x1 = x1 - center_x;
    x2 = x2 - center_x;
    y1 = y1 - center_y;
    y2 = y2 - center_y;
    
    //full equation method
    double slope = a_ray.get_slope();
    double b = slope*-1*x1+y1;
    double disc = (radius*radius)-(b*b)+(slope*slope)*(radius*radius); //(slope*slope+1)*(radius*radius)-(b*b);
    
    if(disc > 0) {
      return true;
    } else {
      return false;
    }  
  }
  
  Coord intersection_coord(Ray a_ray) {
    double x1, y1, x2, y2; 
    double int1_x, int1_y;
    double int2_x, int2_y;
    
    if(a_ray.is_vertical()) {
      return new Coord(a_ray.get_x1(), center_y); 
    }
    
    x1 = a_ray.get_x1();
    y1 = a_ray.get_y1();
    x2 = a_ray.get_x2();
    y2 = a_ray.get_y2();
    // translate points so the circle is at the origin
    x1 = x1 - center_x;
    x2 = x2 - center_x;
    y1 = y1 - center_y;
    y2 = y2 - center_y;
    
    //full equation method
    double m = a_ray.get_slope();
    double b = m * -1 * x1 + y1;
    double disc = (radius*radius)-(b*b)+(m*m)*(radius*radius);
    int1_x = (-1*m*b+sqrt((float) disc))/(1+m*m);
    int2_x = (-1*m*b-sqrt((float) disc))/(1+m*m);
    
    return new Coord(int1_x+center_x, m*int1_x+b+center_y);  
  }
  
  void draw() {
    if(dragging_pipe) {
      fill(128);
    }
    ellipseMode(RADIUS);
    ellipse(map_x(center_x), map_y(center_y), (float) (radius*zoom), (float) (radius*zoom));
  }
  
}

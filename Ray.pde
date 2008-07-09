class Ray {
  double start_x, start_y, end_x, end_y;  
  
  Ray(double x1, double y1, double x2, double y2) {
    start_x = x1;
    start_y = y1;
    end_x   = x2;
    end_y   = y2;
  } 

  void draw(int strength) {
    if(strength == 3) {
      stroke(255,0,0,128);
    } else if (strength == 2) {
      stroke(0,0,255,128);
    } else if(strength == 1) {
      stroke(0,255,0,128);
    } else {
      stroke(255,230,104,192);
    }
    line(map_x(start_x), map_y(start_y), map_x(end_x), map_y(end_y));
  }
  void draw() {
    line(map_x(start_x), map_y(start_y), map_x(end_x), map_y(end_y));
  }
  
  double get_x1() { return start_x; }
  double get_y1() { return start_y; }
  double get_x2() { return end_x; }
  double get_y2() { return end_y; }
  
  boolean is_vertical() {
    if( abs( (float)(end_x - start_x) ) <= 0.0001) {
      return true;
    } else {
      return false;
    }
  }
  
  double get_slope() {
    return (end_y - start_y)/(end_x - start_x); 
  }

  void set_x1(double x) { start_x = x; }
  void set_y1(double y) { start_y = y; }
  void set_x2(double x) { end_x = x; }
  void set_y2(double y) { end_y = y; }
  
  Coord reflect_point(Coord p) {
    double ml = get_slope();
    double mr = 1.0 / ml * -1.0; 
    double px, py; // starting point
    double xi, yi; // intersection point
    double xp, yp; // reflected point, x & y prime
    
    px = p.getX();
    py = p.getY();
    
    xi = (mr*px - py - ml*start_x + start_y)/(mr-ml);
    yi = ml*(xi - start_x) + start_y;
    
    //line(map_x(px), map_y(py), map_x(xi), map_y(yi)); 
    xp = px + 2.0*(xi - px);
    yp = py + 2.0*(yi - py);
    
    return new Coord(xp, yp);
  }
}

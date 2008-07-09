class Reflector {
  double x_min, x_max, range, a;
  Ray gate_line;
  int res = 100;
  
  Reflector(double A, double minimum, double maximum) {
    a = A;
    x_min = minimum;
    x_max = maximum;
    range = x_max - x_min;
    gate_line = new Ray(x_min, func(x_min), x_max, func(x_max));

  }
  
  double func(double x) { //one (or more) equations describing the reflector shape
    return a*cosh(x)-a; 
    //return a*(x*x);
  }

  double d_func(double x) { // the derivative function of func()
    return a*sinh(x);
    //return a*2*x;
  }
  
  Coord intersection_coord(Ray a_ray) {
    double xi, yi;
    
    if(a_ray.is_vertical()) {
      return new Coord( a_ray.get_x1() , func(a_ray.get_x1()) );
    } else {
      double ax1, ay1, ax2, ay2; //a_ray's points
      double xp1, xp2; // x point 1 and two, intersection areas
      double dist_xp1, dist_xp2; // distance to possible intersection points from the starting point of ray
      double am = a_ray.get_slope();
      int last_sign, cur_sign;

      ax1 = a_ray.get_x1();
      ay1 = a_ray.get_y1();
      ax2 = a_ray.get_x2();
      ay2 = a_ray.get_y2();  
      xp1 = xp2 = -10000.0;
      
      if(ax1 < ax2) { // what direction is the ray facing?
        last_sign = sign(func(ax1+0.001) - (am*((ax1+0.001)-ax1)+ay1) );
        for(double x = (ax1+0.01); x <= x_max; x += range/1000.0) { //trace the ray from just after its origin
          cur_sign = sign(func(x) - (am*(x-ax1)+ay1));
          //stroke(255,0,0);
          //line(map_x(x), map_y(0.0), map_x(x), map_y(func(x) - (am*(x-ax1)+ay1)) );
          if(cur_sign != last_sign) { // change in sign means interection
            if(xp1 <= -1000.0) { // xp1 is empty still, and it isn't the start point
              xp1 = x;
            } else {
              xp2 = x;
            }
          }
          last_sign = cur_sign;
        }
      } else {
        last_sign = sign(func(ax1-0.001) - (am*((ax1-0.001)-ax1)+ay1) );
        for(double x = (ax1-0.01); x >= x_min; x -= range/1000.0) {
          cur_sign = sign(func(x) - (am*(x-ax1)+ay1));
          //stroke(255,0,0);
          //line(map_x(x), map_y(0.0), map_x(x), map_y(func(x) - (am*(x-ax1)+ay1)) );
          if(cur_sign != last_sign) { // change in sign means interection
            if(xp1 <= -1000.0) { // xp1 is empty still, and it isn't the start point
              xp1 = x;
            } else {
              xp2 = x;
            }
          }
          last_sign = cur_sign;
        }        
      }
      
      // the if() below needs to get the intersection point
      if(xp2 <= -1000.0) { // xp1 is the only intersection
        xi = xp1;
        yi = func(xp1);
      } else { // threre are two, pick closest
        dist_xp1 = sqrt(pow((float) (ax1 - xp1),2) + pow((float) (ay1 - func(xp1)), 2));
        dist_xp2 = sqrt(pow((float) (ax1 - xp2),2) + pow((float) (ay1 - func(xp2)), 2));
        if(dist_xp1 < dist_xp2 && (abs((float)dist_xp1) <= 0.01)) {
          xi = xp1;
          yi = func(xp1);
        } else {
          xi = xp2;
          yi = func(xp2);
        }
      }
      //stroke(255,0,0);
      //line(map_x(xi), map_y(0.0), map_x(xi), map_y(func(xi)) );
      return new Coord(xi,yi);
    }
  }

  void draw() {
    stroke(102);
    float x1 = map_x(x_min);
    float y1 = map_y(func(x_min));
    float x2, y2;
    for(double x = x_min; x <= x_max; x = x + (range / res) ) { 
      x2 = map_x(x);
      y2 = map_y(func(x));
      line(x1, y1, x2, y2);
      x1 = x2;
      y1 = y2;
    }
  }
  
  boolean intersects(Ray a_ray) {
    if(a_ray.is_vertical()) {
      if(a_ray.get_x1() < x_min) {
        return false;
      }
      if(a_ray.get_x1() > x_max) {
        return false;
      }   
      return true; 
    } else {
      double ax1, ay1, ax2, ay2; //a_ray's start points
      int last_sign, cur_sign;
      double am = a_ray.get_slope();
      
      ax1 = a_ray.get_x1();
      ay1 = a_ray.get_y1();
      ax2 = a_ray.get_x2();
      ay2 = a_ray.get_y2();
 

      if(ax1 < ax2) {
        last_sign = sign(func(ax1+0.01) - (am*((ax1+0.01)-ax1)+ay1) );
        for(double x = (ax1+0.01); x <= x_max; x += range/1000.0) {
          cur_sign = sign(func(x) - (am*(x-ax1)+ay1));
          if(cur_sign != last_sign) { // change in sign means interection
            return true;
          }
          last_sign = cur_sign;
        }
      } else {
        last_sign = sign(func(ax1-0.01) - (am*((ax1-0.01)-ax1)+ay1) );
        for(double x = (ax1-0.01); x >= x_min; x -= range/1000.0) {
          cur_sign = sign(func(x) - (am*(x-ax1)+ay1));
          if(cur_sign != last_sign) { // change in sign means interection
            return true;
          }
          last_sign = cur_sign;
        }
      }
      return false;
    } // end non-vertical intersection test
  }
  
  Ray reflected_ray(Ray a_ray) { 
    double ax1, ay1, ax2, ay2; //a_ray's points
    double bx1, by1, bx2, by2; //reflected ray's points
    
    ax1 = a_ray.get_x1();
    ay1 = a_ray.get_y1();
    ax2 = a_ray.get_x2();
    ay2 = a_ray.get_y2();
    
    if(a_ray.is_vertical()) { // only works for vertical incoming rays
      //first point is coord of intersection 
      bx1 = ax1;       
      by1 = func(ax1); 
      
      float shift = (ax1>0)?PI/2:-1*PI/2;
      float theta = atan((float)d_func(ax1))*2-shift;
      double r_slope = tan(theta);
      double d_x = 20*cos(theta);
      double d_y = 20*sin(theta);
      
      if(ax1<=0) {
        bx2 = ax1 + d_x;
        by2 = func(ax1) + d_y;
      } else {
        bx2 = ax1 - d_x;
        by2 = func(ax1) - d_y;
      }
      
      return new Ray(bx1, by1, bx2, by2);
    } else { // not a vertical ray
      Coord r_start, r_end; // coords of reflected endpoints of a_ray
      Coord isect;
      double xi, yi;
      Ray tangent;
      double fm; //function slope at intersection
      
      isect = intersection_coord(a_ray);
      xi = isect.getX();
      yi = isect.getY();
      
      fm = d_func(xi);
      tangent = new Ray(xi-1.0, fm*((xi-1.0)-xi)+yi, xi+1.0, fm*((xi+1.0)-xi)+yi );
      //stroke(128);
      //tangent.draw();
      // reflect the start and end points over the tangent ray
      r_start = tangent.reflect_point(new Coord(ax1, ay1));
      r_end = tangent.reflect_point(new Coord(ax2, ay2));
      
      // reflected start and end points are the reflected ray, return it-
      return new Ray( r_start.getX(), r_start.getY(), r_end.getX(), r_end.getY() );
    }
  }
  
  double cosh(double x) {
    return (Math.exp(x)+Math.exp(-x))/2.0;
  }
  double sinh(double x) {
    return (Math.exp(x)-Math.exp(-x))/2.0;
}
  int sign(double value) {
    if(value < 0) 
      return -1;
     else
       return 1;
  }
}

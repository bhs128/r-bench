class Coord {
  double x, y;
	
  Coord(double x1, double y1) {
    x = x1;
    y = y1;
  }
	
  void setXY(double x1, double y1) {
     x = x1;
     y = y1;
  }
  double getX() {
    return x;
  }

  double getY() {
    return y;
  }
  
  void draw() {
    point(map_x(x), map_y(y));
  }
}

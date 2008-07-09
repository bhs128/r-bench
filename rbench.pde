Reflector cat;
double x_range = 6.0;
double y_range;
Ray[] sun_array = new Ray[1000];

void setup() {
  size(640,480);
  smooth();
  y_range = x_range *  height / width;

  fill_sun_array(90.0,0.03125);
  cat = new Reflector(0.3, -2.5, 2.5);
  noLoop();
}

void draw() {
  draw_graphpaper();
  Ray temp, temp2, temp3;
  Coord pivot, pivot2, pivot3;

  for(int ridx = 0; ridx < 500; ridx++) {
  // int ridx = 15;
   if(sun_array[ridx] != null) {
     if(cat.intersects(sun_array[ridx])) {
       temp = cat.reflected_ray(sun_array[ridx]);
       pivot = cat.intersection_coord(sun_array[ridx]);
       sun_array[ridx].set_x2(pivot.getX());
       sun_array[ridx].set_y2(pivot.getY());
       temp.set_x1(pivot.getX());
       temp.set_y1(pivot.getY()); 

       if(cat.intersects(temp)) {
         temp2 = cat.reflected_ray(temp);
         pivot2 = cat.intersection_coord(temp);
         temp.set_x2(pivot2.getX());
         temp.set_y2(pivot2.getY());
         temp2.set_x1(pivot2.getX());
         temp2.set_y1(pivot2.getY()); 
         if(cat.intersects(temp2)) {
           temp3 = cat.reflected_ray(temp2);
           pivot3 = cat.intersection_coord(temp2);
           temp2.set_x2(pivot3.getX());
           temp2.set_y2(pivot3.getY());
           temp3.set_x1(pivot3.getX());
           temp3.set_y1(pivot3.getY());         
           temp3.draw(3);
         }
         temp2.draw(2);
       }
//       */
       temp.draw(1);
     }
     sun_array[ridx].draw(0);
   }
  }
  strokeWeight(2);  
  cat.draw();
  strokeWeight(1);
      //saveFrame("frame-####.png"); 
}

void draw_graphpaper() {
  background(255);
  stroke(51,153,255);
  // Draw x-axis
  line(0.0, map_y(0.0), width, map_y(0.0));
  // Draw y-axis
  line(map_x(0.0), 0.0, map_x(0.0), height);    
}

void fill_sun_array(double theta, double spacing) {
  float px_spacing = (float)(width*spacing/x_range);
  float num_rays = width / px_spacing;
  Ray temp;
  double len = sqrt(pow((float) x_range, 2) + pow((float) y_range, 2))*2.0;
  
  if(theta == 90.0) {
    for(int idx = 0; idx < num_rays; idx++) {
      temp = new Ray(idx*spacing-x_range/2.0, y_range, idx*spacing-x_range/2.0, -1*y_range);
      //temp.draw();
      sun_array[idx] = temp;
    }
  } else if(theta < 90.0){
    theta = radians((float)theta);
    double x_inc = spacing / sin((float)theta);
    double ay = (y_range/10.0*9.0);
    double by = -1.0*(y_range/10.0);
    double px = ay / tan((float)theta);
    double nx = by / tan((float)theta);
    num_rays = (float)((x_range+px-nx)/x_inc);
    
    for(int idx = 0; idx <= num_rays; idx++) {
      temp = new Ray(x_inc*idx+px-(x_range)/2.-px, ay, x_inc*idx+nx-(x_range)/2.-px, by*10);
      sun_array[idx] = temp;
    } 
  }
}

float map_x(double x) {
  return map((float) x, (float) x_range/2.0*-1, (float) x_range/2.0, 0, width);
}
float map_y(double y) {
  return map((float) y, (float) y_range/10.0*-1, (float) y_range/10.0*9.0, height, 0);
}  

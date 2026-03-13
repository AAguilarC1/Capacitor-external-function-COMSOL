#ifndef AREA_H
#define AREA_H
#include <math.h>

static inline double area_rectangle(double length, double width){
  return length * width;
}

static inline double area_circle(double radius){
  return M_PI * radius * radius;
}

static inline double area_pill(double length, double width){
  const double L_r = length - width; 
  const double radius = width / 2.0;
  double area_rect = area_rectangle(L_r, width);
  double area_circ = area_circle(radius); 
  return area_rect + area_circ;
}

#endif // AREA_H

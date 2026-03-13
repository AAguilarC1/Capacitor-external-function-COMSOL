#ifndef EQUIVALENT_H
#define EQUIVALENT_H
#include <math.h>

static inline double R_Equivalent(double area){
  return sqrt(area / M_PI);
}

static inline double L_Equivalent(double area) {
  return sqrt(area);
}

static inline double L_W_Equivalent(double area, double width) {
  return area/width;
}

#endif // EQUIVALENT_H

#ifndef ALGEBRA_H
#define ALGEBRA_H

#include "ruta.h"

double map_v(double d);
double map_d(double v);
double map_h(double h0, double v);

void offset(double *offset_table, const ruta_t *ruta, double ym, double xm);

#endif    // ALGEBRA_H
#ifndef _MATH_H_
#define _MATH_H_

#include <math.h>
typedef struct {
	double x;
	double y;
}lee_point_t;

double sqrt(double x);;
static inline double lee_sqrt(double x)
{
	return sqrt(x);
}

extern void sincos(double d, double* s, double* c);
static inline void lee_sincos(double degree, double* sin, double* cos)
{
	sincos(degree, sin, cos);
}

#endif

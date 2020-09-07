#ifndef _BASE_MATH_H_
#define _BASE_MATH_H_

#include "util/types.h"

typedef struct {
	double x;
	double y;
	double z;
} info_point_t;

double util_sin(double x);

double util_cos(double x);

void util_sincos(double d, double *s, double* c);

double util_asin(double x);

double util_acos(double x);

double util_sqrt(double x);

double util_tan(double d);

double util_atan(double d);

double util_log(double d);

double util_fabs(double d);

double util_round(double x);

#endif


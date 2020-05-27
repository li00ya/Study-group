#include "util/math.h"

#include <math.h>

extern void sincos(double d, double* s, double* c);

double util_sin(double x)
{
	return sin(x);
}

double util_cos(double x)
{
	return cos(x);
}

void util_sincos(double d, double *s, double* c)
{
	sincos(d, s, c);
}

double util_asin(double x)
{
	return asin(x);
}

double util_acos(double x)
{
	return acos(x);
}

double util_sqrt(double x)
{
	return sqrt(x);
}

double util_tan(double d)
{
	return tan(d);
}

double util_atan(double d)
{
	return atan(d);
}

double util_log(double d)
{
	return log(d);
}

double util_fabs(double d)
{
	return fabs(d);
}

double util_round(double x)
{
	return round(x);
}

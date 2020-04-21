#include <stdio.h>
#include <math.h>
#include <stdint.h>

typedef struct
{
	double x;
	double y;
}point_info_t;

typedef struct
{
	point_info_t *start;
	point_info_t *end;
}line_vector_t;

static inline void vector_sub(point_info_t* a, point_info_t* b, point_info_t* ret)
{
	ret->x = b->x - a->x;
	ret->y = b->y - a->y;
}

static inline double vector_modulo(point_info_t* a)
{
	return sqrt(a->x * a->x + a->y * a->y);
}

int32_t get_intersection(line_vector_t* line1, line_vector_t* line2, point_info_t* ret)
{   
	double p, q, delt;
	point_info_t l1, l2;
	
	vector_sub(line1->end, line1->start, &l1);
	vector_sub(line2->end, line2->start, &l2);
	
	p = line1->start->x * line1->end->y - line1->start->y * line1->end->x;
	q = line2->start->x * line2->end->y - line2->start->y * line2->end->x;
	
	delt = l1.x * l2.y - l1.y * l2.x;
	
	if (fabs(delt) < 0.01) {
	    return -1;
	}
	
	ret->x = (p * l2.x - q * l1.x) / delt;
	ret->y = (p * l2.y - q * l1.y) / delt;
	
	return 0;
}

#define PI	3.1415926535898
double get_angle(line_vector_t* line1, line_vector_t* line2)
{
	double denominator, molecular, cos, thet;
	point_info_t l1, l2;
	
	vector_sub(line1->end, line1->start, &l1);
	vector_sub(line2->end, line2->start, &l2);

/*
	cos = |a * b| / |a||b| 
		= (ax * bx + ay * by) / (sqrt(ax * ax + ay * ay) * sqrt(bx * bx + by * by)) 
*/
	molecular = l1.x * l2.x + l1.y * l2.y;
	denominator = vector_modulo(&l1) * vector_modulo(&l2);

	cos = molecular / denominator;
	thet = acos(cos) * (180 / PI);
		
	return thet > 90 ? 180 - thet : thet;
}

int32_t main(void)
{
	point_info_t p1, p2, p3, p4, ret;
	line_vector_t l1, l2;
	double tmp;

	p1.x = 2.0;
    p1.y = 2.0;
    p2.x = 7.0;
    p2.y = 7.0;

	p3.x = 1;
	p3.y = 4;
	p4.x = 6;
	p4.y = -1;

	l1.start = &p1;
	l1.end = &p2;
	l2.start = &p3;
	l2.end = &p4;

	tmp = get_angle(&l1, &l2);
	printf("angle %lf\n", tmp);

	get_intersection(&l1, &l2, &ret);
	printf("intersection %lf, %lf\n", ret.x, ret.y);
}


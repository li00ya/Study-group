#include "util/math.h"
#include "util/types.h"
#include "util/string.h"
#include "api/mode.h"

typedef struct
{
	info_point_t *start;
	info_point_t *end;
}line_vector_t;

static inline void vector_sub(info_point_t* a, info_point_t* b, info_point_t* ret)
{
	ret->x = b->x - a->x;
	ret->y = b->y - a->y;
}

static inline double vector_modulo(info_point_t* a)
{
	return util_sqrt(a->x * a->x + a->y * a->y);
}

static int32_t get_intersection(line_vector_t* line1, line_vector_t* line2, info_point_t* ret)
{   
	double p, q, delt;
	info_point_t l1, l2;
	
	vector_sub(line1->end, line1->start, &l1);
	vector_sub(line2->end, line2->start, &l2);
	
	p = line1->start->x * line1->end->y - line1->start->y * line1->end->x;
	q = line2->start->x * line2->end->y - line2->start->y * line2->end->x;
	
	delt = l1.x * l2.y - l1.y * l2.x;
	
	if (util_fabs(delt) < 0.01) {
	    return -1;
	}
	
	ret->x = (p * l2.x - q * l1.x) / delt;
	ret->y = (p * l2.y - q * l1.y) / delt;
	
	return 0;
}

#define PI	3.1415926535898
static double get_angle(line_vector_t* line1, line_vector_t* line2)
{
	double denominator, molecular, cos, thet;
	info_point_t l1, l2;
	
	vector_sub(line1->end, line1->start, &l1);
	vector_sub(line2->end, line2->start, &l2);

/*
	cos = |a * b| / |a||b| 
		= (ax * bx + ay * by) / (sqrt(ax * ax + ay * ay) * sqrt(bx * bx + by * by)) 
*/
	molecular = l1.x * l2.x + l1.y * l2.y;
	denominator = vector_modulo(&l1) * vector_modulo(&l2);

	cos = molecular / denominator;
	thet = util_acos(cos) * (180 / PI);
		
	return thet > 90 ? 180 - thet : thet;
}

static int32_t cross_proc(int32_t ac, int8_t** av)
{
	info_point_t p1, p2, p3, p4, ret;
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
	util_puts("angle %lf\n", tmp);

	get_intersection(&l1, &l2, &ret);
	util_puts("intersection %lf, %lf\n", ret.x, ret.y);

	return 0;
}

static int32_t cross_usage(void)
{
	util_puts("cross option:\n");
	util_puts("\t\n");

	return 0;
}

static lee_module_t cross_module = {
	.name = "cross",
	.usage = cross_usage,
	.proc = cross_proc,
};

static void PREP_INIT cross_constructor(void)
{
	lee_module_register(LEE_TYPE_ALG, LEE_ALG_CROSS, &cross_module);
}

static void PREP_EXIT cross_destructor(void)
{
	lee_module_unregister(LEE_TYPE_ALG, LEE_ALG_CROSS);
}

#include "util/string.h"
#include "util/math.h"
#include "util/types.h"
#include "api/mode.h"

/*快速求直角三角形非直角点坐标		       h
*参数：                                   |\
*	r：直角点坐标                         | \
*	h：直角三角形另一点坐标               |  \
*	ret:直角三角形待求的未知点坐标        |___\
*	len: 角h所对应的直角边长,             r   ret
		注意：ret在rh两点连线的上边或者右边，len取值为负; 在下边或者左边，len取值为正。
*返回值：
*	0，成功
*原始公式：
*	前向交会(https://blog.csdn.net/weixin_42282031/article/details/105495058)
*简化公式：
*	x = xa + (ya - yb) * tanB
*	y = ya + (xb - xa) * tanB
*/


int32_t simplified_forward_intersection(info_point_t* r, info_point_t* h, info_point_t* ret, double len)
{
	double tan;
	double x = r->x - h->x;
	double y = r->y - h->y;
	double dis = util_sqrt(x * x + y * y);

	if (dis < 0.001) {
		return -1;
	}

	tan = len / dis;

	ret->x = r->x + y * tan;
	ret->y = r->y - x * tan;

	return 0;
}

static int32_t grid_proc(int32_t argc, int8_t** argv)
{
	info_point_t a, b, c;
	double len = 1.8;

	a.x = 3;
	a.y = 2;
	b.x = 3;
	b.y = 5;

	simplified_forward_intersection(&a, &b, &c, len);
	util_puts("c: x %lf, y %lf\n", c.x, c.y);

	return 0;
}

static int32_t grid_usage(void)
{
	util_puts("grid option:\n");
	util_puts("\t\n");

	return 0;
}

static lee_module_t grid_module = {
	.name = "grid",
	.usage = grid_usage,
	.proc = grid_proc,
};

static void PREP_INIT grid_constructor(void)
{
	lee_module_register(LEE_TYPE_ALG, LEE_ALG_GRID, &grid_module);
}

static void PREP_EXIT grid_destructor(void)
{
	lee_module_unregister(LEE_TYPE_ALG, LEE_ALG_GRID);
}

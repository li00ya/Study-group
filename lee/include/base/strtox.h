#ifndef _STRTOX_H_
#define _STRTOX_H_

#include "util/types.h"

#if CONFIG_BITS_PER_LONG == 32
#else

/**
 * div_u64_rem - unsigned 64bit divide with 32bit divisor with remainder
 *
 * This is commonly provided by 32bit archs to provide an optimized 64bit
 * divide.
 */
static inline uint64_t div_u64_rem(uint64_t dividend, uint32_t divisor, uint32_t *remainder)
{
	*remainder = dividend % divisor;
	return dividend / divisor;
}

/**
 * div_s64_rem - signed 64bit divide with 32bit divisor with remainder
 */
static inline int64_t div_s64_rem(int64_t dividend, int32_t divisor, int32_t *remainder)
{
	*remainder = dividend % divisor;
	return dividend / divisor;
}

/**
 * div64_u64_rem - unsigned 64bit divide with 64bit divisor and remainder
 */
static inline uint64_t div64_u64_rem(uint64_t dividend, uint64_t divisor, uint64_t *remainder)
{
	*remainder = dividend % divisor;
	return dividend / divisor;
}

/**
 * div64_u64 - unsigned 64bit divide with 64bit divisor
 */
static inline uint64_t div64_u64(uint64_t dividend, uint64_t divisor)
{
	return dividend / divisor;
}

/**
 * div64_s64 - signed 64bit divide with 64bit divisor
 */
static inline int64_t div64_s64(int64_t dividend, int64_t divisor)
{
	return dividend / divisor;
}
#endif

static inline uint64_t div_u64(uint64_t dividend, uint32_t divisor)
{
	uint32_t remainder;
	return div_u64_rem(dividend, divisor, &remainder);
}
/**
 * div_s64 - signed 64bit divide with 32bit divisor
 */
static inline int64_t div_s64(int64_t dividend, int32_t divisor)
{
	int32_t remainder;
	return div_s64_rem(dividend, divisor, &remainder);
}

const int8_t *parse_integer_base(const int8_t *s, uint32_t *base);

/**
 * simple_strtoull - convert a string to an unsigned long long
 * @cp: The start of the string
 * @endp: A pointer to the end of the parsed string will be placed here
 * @base: The number base to use
 *
 * This function is obsolete. Please use kstrtoull instead.
 */
uint64_t util_strtoull(const void* cp, void** endp, uint32_t base);

/**
 * util_strtoul - convert a string to an unsigned long
 * @cp: The start of the string
 * @endp: A pointer to the end of the parsed string will be placed here
 * @base: The number base to use
 *
 * This function is obsolete. Please use kstrtoul instead.
 */
uint32_t util_strtoul(const void* cp, void** endp, uint32_t base);

/**
 * util_strtol - convert a string to a signed long
 * @cp: The start of the string
 * @endp: A pointer to the end of the parsed string will be placed here
 * @base: The number base to use
 *
 * This function is obsolete. Please use kstrtol instead.
 */
int32_t util_strtol(const void* cp, void** endp, uint32_t base);

/**
 * util_strtoll - convert a string to a signed long long
 * @cp: The start of the string
 * @endp: A pointer to the end of the parsed string will be placed here
 * @base: The number base to use
 *
 * This function is obsolete. Please use kstrtoll instead.
 */
int64_t util_strtoll(const void* cp, void** endp, uint32_t base);

double util_strtod(const void* str, void** endptr);

float util_strtof(const void *str, void** endptr);

int32_t util_atoi(const void *nptr);

#endif


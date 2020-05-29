#include "base/ctype.h"
#include "base/strtox.h"
#include <stdlib.h>

#define KSTRTOX_OVERFLOW	(1U << 31)

const int8_t *parse_integer_base(const int8_t *s, uint32_t *base)
{
	if (*base == 0) {
		if (s[0] == '0') {
			if (_tolower(s[1]) == 'x' && isxdigit(s[2]))
				*base = 16;
			else
				*base = 8;
		} else
			*base = 10;
	}
	if (*base == 16 && s[0] == '0' && _tolower(s[1]) == 'x')
		s += 2;
	return s;
}

/*
 * Convert non-negative integer string representation in explicitly given radix
 * to an integer.
 * Return number of characters consumed maybe or-ed with overflow bit.
 * If overflow occurs, result integer (incorrect) is still returned.
 *
 * Don't you dare use this function.
 */
static uint32_t parse_integer(const int8_t *s, uint32_t base, uint64_t *p)
{
	uint64_t res;
	uint32_t rv;

	res = 0;
	rv = 0;
	while (1) {
		uint32_t c = *s;
		uint32_t lc = c | 0x20; /* don't tolower() this line */
		uint32_t val;

		if ('0' <= c && c <= '9') {
			val = c - '0';
		} else if ('a' <= lc && lc <= 'f') {
			val = lc - 'a' + 10;
		} else {
			break;
		}

		if (val >= base) {
			break;
		}
		/*
		 * Check for overflow only if we are within range of
		 * it in the max base we support (16)
		 */
		if (res & (~0ull << 60)) {
			if (res > div_u64(~0ULL - val, base)) {
				rv |= KSTRTOX_OVERFLOW;
			}
		}
		res = res * base + val;
		rv++;
		s++;
	}
	*p = res;
	return rv;
}

uint64_t util_strtoull(const void* cp, void** endp, uint32_t base)
{
	uint64_t result;
	uint32_t rv;

	cp = parse_integer_base(cp, &base);
	rv = parse_integer(cp, base, &result);
	/* FIXME */
	cp += (rv & ~KSTRTOX_OVERFLOW);

	if (endp) {
		*endp = (void*)cp;
	}

	return result;
}

uint32_t util_strtoul(const void* cp, void** endp, uint32_t base)
{
	return util_strtoull(cp, endp, base);
}

int32_t util_strtol(const void* cp, void** endp, uint32_t base)
{
	int8_t byte = *(int8_t*)cp;
	
	if ('-' == byte) {
		return -util_strtoul(cp + 1, endp, base);
	}

	return util_strtoul(cp, endp, base);
}

int64_t util_strtoll(const void* cp, void** endp, uint32_t base)
{
	int8_t byte = *(int8_t*)cp;

	if ('-' == byte) {
		return -util_strtoull(cp + 1, endp, base);
	}
	
	return util_strtoull(cp, endp, base);
}

double util_strtod(const void *str, void** endptr)
{
	return strtod(str, (int8_t**)endptr);
}

float util_strtof(const void *str, void** endptr)
{
	return strtof(str, (int8_t**)endptr);
}

int32_t util_atoi(const void *nptr)
{
	return atoi(nptr);
}


#ifndef _MACRO_H_
#define _MACRO_H_

#define FHS1(a)  ((a) ? 1 : 0)
#define FHS2(a)  (((a) >> 1) ? (1 + FHS1((a) >> 1)) : FHS1((a) & 1))
#define FHS4(a)  (((a) >> 2) ? (2 + FHS2((a) >> 2)) : FHS2((a) & 3))
#define FHS8(a)  (((a) >> 4) ? (4 + FHS4((a) >> 4)) : FHS4((a) & 0xf))
#define FHS16(a) (((a) >> 8) ? (8 + FHS8((a) >> 8)) : FHS8((a) & 0xff))
#define FHS32(a) (((a) >> 16) ? (16 + FHS16((a) >> 16)) : FHS16((a) & 0xffff))

#endif

#ifndef __HASHFUNC_H__
#define __HASHFUNC_H__

#include <stdint.h>

typedef void (HashFunc)(const void *key, int len, uint32_t seed, void *out);

#endif //__HASHFUNC_H__

#ifndef __DEFS_H__
#define __DEFS_H__

#include <stdint.h>

typedef struct 
{
    uint8_t g,r,b;
} color_t;

typedef struct 
{
    uint32_t g,r,b;
} color32_t;

void  dprintf(const char *fmt, ...);
#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

#endif

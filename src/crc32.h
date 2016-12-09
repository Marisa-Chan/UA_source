#ifndef CRC32_H_INCLUDED
#define CRC32_H_INCLUDED

#include <inttypes.h>
#include <stdlib.h>

uint32_t crc32(uint32_t crc, const void *buf, size_t size);

#endif

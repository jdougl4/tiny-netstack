#ifndef UTIL_H
#define UTIL_H

/*
 * util.h
 *
 * Some general helper functions
 */

#include <stddef.h>
#include <stdint.h>

/*
 * Print raw bytes as hexdump
 */
void hex_dump(const uint8_t *data, size_t len);

#endif

/*
 * util.c
 *
 * Utility/helper functions
 */

#include <stdio.h>
#include <ctype.h>

#include "util.h"

/*
 * Print buffer in classic hexdump format
 */
void hex_dump(const uint8_t *data, size_t len) {
	
	for (size_t i = 0; i < len; i += 16) {
		// Print byte offset
		printf("%04zx  ", i);

		// Hex section
		for (size_t j = 0; j < 16; j++) {

			if (i + j < len) {

				uint8_t c = data[i + j];

				if (isprint(c)) {
					printf("%c", c);
				} else {
					printf(".");
				}
			}
		}

		printf("\n");
	}
}

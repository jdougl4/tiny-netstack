/*
 * main.c
 *
 * Current goal functionality is to open a TAP interface, receive raw 
 * Ethernet frames, print frame info, and hexdump frame contents
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <net/if.h>

#include "tap.h"
#include "util.h"
#include "ethernet.h"

int main(void) {
	/* 
	 * TAP interface name. Kernel may overwrite this if needed
	 * IFNAMSIZ (interface name size) is defined in net/if.h as a 16-byte
	 * constant, so 15 user-visible bytes (assuming it includes a 
	 * trailing null)
	 */ 
	char tap_name[IFNAMSIZ] = "tap0";

	/*
	 * Open TAP interface
	 *
	 * tap_fd = "tap file descriptor"
	 *
	 * tap_open expects a character array, so we pass tap_name.
	 */
	int tap_fd = tap_open(tap_name);

	if (tap_fd < 0) {
		fprintf(stderr, "Failed to open TAP interface\n");
		return EXIT_FAILURE;
	}

	printf("[+] Opened TAP interface: %s\n", tap_name);

	/*
	 * Packet receive buffer
	 *
	 * TAP_BUFFER_SIZE = max size of an Ethernet frame
	 */
	uint8_t buffer[TAP_BUFFER_SIZE];

	// Main receive loop
	while (1) {

		/*
		 * tap_read reads Ethernet frames from a TAP interface
		 *
		 * it accepts a TAP file descriptor, a memory buffer where 
		 * packet bytes will be stored, and a maximum number of bytes
		 * to read
		 *
		 * tap_read returns the number of bytes read.
		 */
		ssize_t bytes_read = 
			tap_read(tap_fd, buffer, sizeof(buffer));

		if (bytes_read < 0) {
			perror("tap_read");
			continue;
		}

		printf("\n[+] Received Ethernet frame (%zd bytes)\n",
				bytes_read);

		// Parse/display Ethernet header
		ethernet_handle_frame(buffer, bytes_read);

		// Print raw packet bytes
		printf("\nHex Dump:\n");

		hex_dump(buffer, bytes_read);
	}

	return EXIT_SUCCESS;
}

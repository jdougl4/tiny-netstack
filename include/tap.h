#ifndef TAP_H
#define TAP_H

/*
 * tap.h
 *
 * TAP interface helper functions
 *
 * TAP devices operate at Layer 2 and provide raw Ethernet frames
 */

#include <stddef.h>
#include <stdint.h>

/*
 * Maximum Ethernet frame size.
 *
 * Standard MTU: 1500 byte payload
 *
 * Plus some extra for the Ethernet header and any possible overhead
 */
#define TAP_BUFFER_SIZE 1600

/*
 * Open or attach to a TAP interface
 *
 * Parameters:
 * 	devname - desired TAP interface name
 *
 * Returns:
 * 	file descriptor on success
 * 	-1 on failure
 */
int tap_open(char *devname);

/*
 * Read a frame from TAP interface
 *
 * Returns:
 * 	number of bytes read
 * 	-1 on failure
 */
ssize_t tap_read(int fd, uint8_t *buffer, size_t size);

#endif

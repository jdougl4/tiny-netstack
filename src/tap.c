/*
 * tap.c
 *
 * Handles Linux TAP device interaction
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/ioctl.h>

#include <linux/if.h>
#include <linux/if_tun.h>

#include "tap.h"

/*
 * Open or create TAP interface
 */
int tap_open(char *devname) {
	struct ifreq *ifr;

	
	// Open TUN/TAP clone device
	int fd = open("/dev/net/tun", O_RDWR);

	if (fd < 0) {
		perror("open(/dev/net/tun)");
		return -1;
	}

	// Clear interface request structure
	memset(&ifr, 0, sizeof(ifr));

	/*
	 * Configure:
	 * 	IFF_TAP -> Layer 2 Ethernet frames
	 * 	IEF_NO_PI -> No extra packet info header
	 */
	ifr.ifr_flags = IFF_TAP | IFF_NO_PI;

	// Copy desired interface name
	strncpy(ifr.ifr_name, devname, IFNAMSIZ - 1);

	// Create/attach TAP interface
	if (ioctl(fd, TUNSETIFF, (void *)&ifr) < 0) {

		perror("ioctl(TUNSETIFF)");

		close(fd);

		return -1;
	}

	/*
	 * Kernel may modify interface name.
	 * Copy actual name back to caller
	 */
	strcpy(devname, ifr.ifr_name);

	return fd;
}

// Read Ethernet frame from TAP device
ssize_t tap_read(int fd, uint8_t *buffer, size_t size) {
	return read(fd, buffer, size);
}

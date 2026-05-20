/*
 * tap.c
 *
 * Handles Linux TAP device interaction
 *
 * TUN and TAP are kernel virtual network devices within Linux environments.
 * TUN carries IP packets, TAP carries Ethernet frames.
 *
 * Linux exposes TAP devices are file descriptors, which means we can use 
 * things like read() and write() on frames the same way they work on files. 
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

// For some device-specific operations
#include <sys/ioctl.h>

// Linux network interface structures
#include <linux/if.h>

// TUN/TAP-specific constants and definitions
#include <linux/if_tun.h>

#include "tap.h"

/*
 * Open or create TAP interface
 *
 * tap_open expects a character array (ex. "tap0")
 * but we pass it as a pointer to a string since the kernel may modify it
 *
 * For instance, if the user requests "tap%d", the kernel may return "tap0"
 *
 * So the function may write back to this memory
 */
int tap_open(char *devname) {

	/*
	 * Linux kernel structure used for network interface configuration
	 * if = interface
	 * req = request
	 */
	struct ifreq *ifr;

	
	/*
	 * open() is a standard Unix syscall. Opens a file/device and returns 
	 * a file descriptor
	 *
	 * 0 -> stdin
	 * 1 -> stdout
	 * 2 -> stderr
	 *
	 * open() can also return values greater than 2, meaning 
	 * "resource <integer>"
	 *
	 * /dev/net/tun is the Linux kernel device for TUN/TAP 
	 * functionality.
	 *
	 * O_RDWR = Open for reading and writing (we want both because 
	 * eventually we will want to receive AND send frames)
	 */
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

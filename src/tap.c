/*
 * tap.c
 *
 * Handles Linux TAP device interaction
 *
 * TUN and TAP are kernel virtual network devices within Linux environments.
 * TUN carries IP packets, TAP carries Ethernet frames.
 *
 * Linux exposes TAP devices as file descriptors, which means we can use 
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
	 * functionality. We have to open this and issue a corresponding 
	 * ioctl() to register a network device witht he kernel
	 *
	 * O_RDWR = Open for reading and writing (we want both because 
	 * eventually we will want to receive AND send frames)
	 */
	int fd = open("/dev/net/tun", O_RDWR);
	
	/*
	 * perror prints a message on stderr describing the last error 
	 * encountered during a call to a system or library function
	 */
	if (fd < 0) {
		perror("open(/dev/net/tun)");
		return -1;
	}

	/*
	 * memset() sets memory to a specific byte value. Have to do this 
	 * since stack memory is not automatically initialized
	 *
	 * Before this line, ifr contains garbage bytes that we want to be 0
	 *
	 * &ifr contains the address of an ifr struct
	 *
	 * 0 is the value we want to fill bytes with
	 *
	 * sizeof(ifr) is the number of bytes we want to fill
	 */
	memset(&ifr, 0, sizeof(ifr));

	/*
	 * IFF_TAP is for requesting a Layer 2 TAP device
	 *
	 * IFF_NO_PI (PI = packet information) is required to ensure the kernel
	 * doesn't prepend any extra metadata before the packet. We need good 
	 * ol' unadulterated Ethernet frames
	 *
	 * The bitwise OR | combines these flag bits together
	 */
	ifr.ifr_flags = IFF_TAP | IFF_NO_PI;

	/*
	 * Just copying the interface name into structure
	 *
	 * ifr.ifr_name contains the destination buffer
	 *
	 * devname is our source string from earlier
	 *
	 * IFMANSIZ - 1 is the maximum number of bytes to copy
	 * (-1 for null terminator)
	 */
	strncpy(ifr.ifr_name, devname, IFNAMSIZ - 1);

	/*
	 * Actually registering with the kernel
	 *
	 * fd is our file descriptor
	 *
	 * (void *)&ifr is a pointer to configuration structure
	 *
	 * (void *) designates a generic pointer type. ioctl() accepts this 
	 * because each ioctl takes an argument whose meaning varies depending 
	 * on the operation. Different commands use different structures
	 *
	 * &ifr is the memory address where the ifr structure lives so the 
	 * kernel can read this directly from memory
	 */
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

/*
 * Read Ethernet frame from TAP interface
 *
 * fd is our TAP file descriptor
 *
 * *buffer is the memory buffer where packet bytes will be stored
 *
 * size is the maximum number of bytes to read
 *
 * returns the number of bytes read. A return byte of 42, for instance, 
 * indicates a 42 byte Ethernet frame was received.
 */
ssize_t tap_read(int fd, uint8_t *buffer, size_t size) {
	return read(fd, buffer, size);
}

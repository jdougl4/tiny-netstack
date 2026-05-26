#include <stdio.h>
#include <arpa/inet.h>
#include "ethernet.h"
#include "arp.h"

/*
 * Convert EtherType to a readable format
 *
 * Ex. ethertype = 0x0800 returns "IPv4"
 * ethertype = "0x0806" returns "ARP"
 */
const char *ethernet_ethertype_to_str(uint16_t ethertype) {
	switch (ethertype) {
		case ETHERTYPE_IPV4:
			return "IPv4";

		case ETHERTYPE_IPV6:
			return "IPv6";

		case ETHERTYPE_ARP:
			return "ARP";

		default:
			return "UNKNOWN";
	}
}

/*
 * Pretty print MAC addresses
 *
 * Ex. ab:cd:ef:00:11:22
 *
 * uint8_t is an unsigned 8-bit (1 byte) integer
 * uint8_t *mac is a pointer to the first byte of a MAC address
 */
void ethernet_print_mac(const uint8_t *mac) {

	// This loops through all 6 bytes in a MAC address
	for (int i = 0; i < ETH_ADDR_LEN; i++) {
		// %02x pads with zeroes (if needed) to ensure 
		// a width of 2 characters
		printf("%02x", mac[i]);

		if (i != ETH_ADDR_LEN - 1) {
			printf(":");
		}
	}
}

/*
 * A parser for displaying information about an Ethernet frame
 *
 * frame is a pointer to the first byte of a raw Ethernet frame.
 *
 * This does not pass a whole frame to the function. Just the memory address 
 * where it begins.
 */
void ethernet_handle_frame(const uint8_t *frame, size_t len) {

	// Ensuring the frame is large enough to contain a valid header
	// Must be at least 14 bytes.
	if (len < ETH_HEADER_LEN) {
		fprintf(stderr, "[ETH] Frame too short\n");
		return;
	}

	// Interpreting the beginning of the buffer as the Eth header
	// This defines eth as a pointer to an eth_header struct 
	// (defined in ethernet.h)
	// frame originally has type const uint8_t *, which is a pointer to 
	// a bunch of bytes. 
	// However, in this case we know the first bytes of the frame are 
	// destination MAC, source MAC, and EtherType, which exactly 
	// matches the eth_header struct. 
	const struct eth_header *eth = (const struct eth_header *)frame;

	/*
	 * Converting EtherType from network byte order (big endian) 
	 * into host byte order.
	 *
	 * ntohs() means "netwok to host short". Since network protocols use 
	 * big endian and most CPUs use little endian, we have to use this 
	 * so the bytes don't appear backward to the CPU
	 *
	 * Since eth is a pointer to a struct (not an actual struct object), 
	 * we have to access it through a pointer
	 *
	 * eth->ethertype allows us to go to the struct pointed to by eth and 
	 * access the ethertype field.
	 */
	uint16_t ethertype = ntohs(eth->ethertype);

	printf("\n========== Ethernet Frame ==========\n");

	printf("Destination MAC: ");
	ethernet_print_mac(eth->dst_mac);
	printf("\n");

	printf("Source MAC: ");
	ethernet_print_mac(eth->src_mac);
	printf("\n");

	// %04x pads with zeroes, with a minimum width of 4 characters, in
	// hexadecimal output
	printf("EtherType:       0x%04x (%s)\n",
			ethertype,
			ethernet_ethertype_to_str(ethertype));
	
	// %zu is of type size_t, u indicates unsigned int
	printf("Frame Length:    %zu bytes\n", len);

	printf("===================================\n");

	// Pointer to payload immediately after Ethernet header
	const uint8_t *payload = frame + ETH_HEADER_LEN;

	// Payload length excludes Ethernet header
	size_t payload_len = len - ETH_HEADER_LEN;

	// Dispatch packet based on EtherType
	switch (ethertype) {

		case ETHERTYPE_ARP:

			arp_handle_packet(payload, payload_len);

			break;

		default:

			break;
	}
}

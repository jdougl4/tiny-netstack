#include <stdio.h>
#include <arpa/inet.h>
#include "ethernet.h"

/*
 * Convert EtherType to a readable format
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
 */
void ethernet_print_mac(const uint8_t *mac) {
	for (int i = 0; i < ETH_ADDR_LEN; i++) {
		print("%02x", mac[i]);

		if (i != ETH_ADDR_LEN - 1) {
			printf(":");
		}
	}
}

/*
 * A parser for displaying information about an Ethernet frame
 */
void ethernet_handle_frame(const uint8_t *frame, size_t len) {

	// Ensuring the frame is large enough to contain a valid header
	if (len < ETH_HEADER_LEN) {
		fprintf(stderr, "[ETH] Frame too short\n");
		return;
	}

	// Interpreting the beginning of the buffer as the Eth header
	const struct eth_header *eth = (const struct eth_header *)frame;

	/*
	 * Converting EtherType from network byte order (big endian) 
	 * into host byte order
	 */
	uint16_t ethertype = ntohs(eth->ethertype);

	printf("\n========== Ethernet Frame ==========\n");

	printf("Destination MAC: ");
	ethernet_print_mac(eth->dst_mac);
	printf("\n");

	printf("Source MAC: ");
	ethernet_print_mac(eth->src_mac);
	printf("\n");

	printf("EtherType:       0x%04x (%s)\n",
			ethertype,
			ethernet_ethertype_to_str(ethertype));

	printf("Frame Length:    %zu bytes\n", len);

	printf("===================================\n");
}

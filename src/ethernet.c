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
 * To-Do
 *
 * Implement ethernet_handle_frame()
 */

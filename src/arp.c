/*
 * arp.c
 *
 * Implements ARP packet parsing and display logic
 */

#include <stdio.h>
#include <arpa/inet.h>

#include "arp.h"
#include "ethernet.h"

/*
 * Convert opcode into readable string
 */
const char *arp_opcode_to_str(uint16_t opcode) {
	
	switch (opcode) {

		case ARP_OPCODE_REQUEST:
			return "REQUEST";

		case ARP_OPCODE_REPLY:
			return "REPLY";

		default:
			return "UNKNOWN";

	}
}

/*
 * Print IPv4 address in dotted-decimal notation
 *
 * Example:
 * 	192.168.1.1
 *
 */
void arp_print_ipv4(const uint8_t *ip) {
	
	printf("%u.%u.%u.%u",
			ip[0],
			ip[1],
			ip[2],
			ip[3]);
}

// Parse and display ARP packet
void arp_handle_packet(const uint8_t *packet, size_t len) {

	// Validate minimum ARP size
	if (len < sizeof(struct arp_header)) {
		
		fprintf(stderr, "[ARP] Packet too short\n");

		return;
	}

	// Reinterpret raw packet bytes as ARP header
	const struct arp_header *arp = 
		(const struct arp_header *)packet;

	/*
	 * Convert multi-byte fields from network byte order to host
	 * byte order
	 */
	uint16_t htype = ntohs(arp->htype);
	uint16_t ptype = ntohs(arp->ptype);
	uint16_t opcode = ntohs(arp->opcode);

	printf("\n========== ARP Packet ==========\n");

	printf("Hardware Type:   0x%04x\n", htype);

	printf("Protocol Type: 0x%04x\n", ptype);

	printf("Hardware Length: %u\n", arp->hlen);

	printf("Protocol Length: %u\n", arp->plen);

	printf("Opcode:          %s (%u)\n",
			arp_opcode_to_str(opcode),
			opcode);

	printf("Sender MAC:      ");

	ethernet_print_mac(arp->sender_mac);

	printf("\n");

	printf("Sender IP:       ");

	arp_print_ipv4(arp->sender_ip);

	printf("\n");

	printf("Target MAC:      ");

	ethernet_print_mac(arp->target_mac);

	printf("\n");

	printf("Target IP:       ");

	arp_print_ipv4(arp->target_ip);

	printf("\n");

	printf("================================\n");
}

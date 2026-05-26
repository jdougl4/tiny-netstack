#ifndef ARP_H
#define ARP_H

/*
 * arp.h
 *
 * Responsible for ARP packet structure definitions, ARP constants, and ARP 
 * parser function declarations
 */

#include <stdint.h>
#include <stddef.h>

// ARP hardware type for Ethernet
#define ARP_HTYPE_ETHERNET 0x0001

// Protocol type for IPv4
#define ARP_PTYPE_IPV4 0x0800

// Hardware address length for MAC addresses
#define ARP_HLEN_ETHERNET 6

// Protocol address length for IPv4 addresses
#define ARP_PLEN_IPV4 4

// ARP operation codes
#define ARP_OPCODE_REQUEST 0x0001
#define ARP_OPCODE_REPLY 0x0002

/*
 * Standard ARP packet size for Ethernet + IPv4
 *
 * 2 bytes -> hardware type
 * 2 bytes -> protocol type
 * 1 byte -> hardware length
 * 1 byte -> protocol length
 * 2 bytes -> opcode
 * 6 bytes -> sender MAC
 * 4 bytes -> sender IP
 * 6 bytes -> target MAC
 * 4 bytes -> target IP
 *
 * Total = 28 bytes
 */
#define ARP_PACKET_LEN 28

/*
 * ARP packet structure
 *
 * __attribute__((packed)) prevents compiler adding bytes
 */
struct arp_header {

	uint16_t htype;
	uint16_t ptype;

	uint8_t hlen;
	uint8_t plen;

	uint16_t opcode;

	uint8_t sender_mac[6];
	uint8_t sender_ip[4];

	uint8_t target_mac[6];
	uint8_t target_ip[4];

} __attribute__((packed));

// Parse and display ARP packet information
void arp_handle_packet(const uint8_t *packet, size_t len);

// Convert ARP opcode to readable string
const char *arp_opcode_to_str(uint16_t opcode);

// Print IPv4 address in dotted-decimal format
void arp_print_ipv4(const uint8_t *ip);

#endif

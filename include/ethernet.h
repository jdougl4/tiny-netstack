#ifndef ETHERNET_H
#define ETHERNET_H

/*
 * ethernet.h will contain primarily a bunch of Ethernet protocol constants 
 * and frame header structure
 */
#include <stdint.h>
#include <stddef.h>

/*
 * EtherType is a 2-byte field in an Ethernet frame that acts as a protocol 
 * identifier. This tells the receiving NIC which protocol is encapsulated 
 * within, like IPv4 or IPv6 (or a whole bunch of other stuff)
 */
#define ETHERTYPE_IPV4 0x0800
#define ETHERTYPE_ARP 0x0806
#define ETHERTYPE_IPV6 0x86DD

// MAC addresses are always 6 bytes
#define ETH_ADDR_LEN 6

/*
 * This is the minimum size for an Ethernet header. Source MAC (6 bytes) + 
 * destination MAC (6 bytes) + EtherType (2 bytes) = 14 bytes.
 */
#define ETH_HEADER_LEN 14

/*
 * Frame header
 *
 * This is my first time using a packed struct so I am adding a more verbose 
 * comment here for my own understanding
 *
 * The __attribute__((packed)) bit is a compiler attribute. Both GCC and Clang 
 * specifically support this
 *
 * Basically tells the compiler NOT to add bytes into this struct as padding.
 *
 * Since packet bytes need to be precise at a binary level, can't afford the 
 * compiler adding padding or anything weird like that. I have learned that, 
 * typically, compilers do that because the CPU prefers to read memory at 
 * aligned addresses (ex., 4-byte integers aligned on 4-byte boundaries, 
 * 8-byte values aligned on 8-byte boundaries, etc). This is faster. So the 
 * compiler may sometimes insert "invisible" bytes.
 *
 * As for syntax. __attribute__ takes arguments, and 'packed' itself is an 
 * attribute inside the attribute list.  
 */
struct eth_header {
	uint8_t dst_mac[ETH_ADDR_LEN];
	uint8_t src_mac[ETH_ADDR_LEN];
	uint16_t ethertype;
} __attribute__((packed));

/*
 * Quick parser for Ethernet stuff
 *
 * Params:
 * 	frame - raw Ethernet frame buffer
 * 	len   - total frame length
 */
void ethernet_handle_frame(const uint8_t *frame, size_t len);

/*
 * Quick thing to convert EtherType to readable format
 */
const char *ethernet_ethertype_to_str(uint16_t ethertype);

/*
 * Pretty print a MAC address
 */
void ethernet_print_mac(const uint8_t *mac);

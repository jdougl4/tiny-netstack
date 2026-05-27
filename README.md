tiny-netstack is a project I've started to try to better understand network programming and how networking protocols work at a low level. tiny-netstack is my in-development attempt to write a userspace network stack written in C for Linux using TAP interfaces.

It does not offer any practical functionality in addition to what your kernel probably already gives you. It's more of a learning exercise to teach packet parsing, byte ordering, protocol implementation, Linux networking internals, and a ton of other stuff that I am painfully discovering along the way.

My current implementation supports:
- TAP interface integration
- Ethernet frame reception
- Ethernet header parsing
- EtherType identification
- ARP packet parsing/display

My future goals for the project are:
- ARP replies
- IPv4 parsing
- ICMP echo replies
- Checksum handling
- ARP cache

Build Instrutions:
- Clone and compile with 'make'

TAP Interface Setup:
- Create a TAP interface: sudo ip tuntap add dev tap0 mode tap
- Assign IPv4 address: sudo ip addr add 10.0.0.1/24 dev tap0
- Bring it online: sudo ip link set tap0 up
- Verify: ip addr show tap0
- Delete it: sudo ip link delete tap0

I have hacked out a simple script to automate the TAP interface setup--use it if you'd like. Just remember to run "chmod +x scripts/setup_tap.sh"

To Run: sudo ./tiny-netstack

I recommend opening a separate terminal and pinging your TAP interface while the program is running. Depending on your kernel's networking state/ARP cache contents you may see ARP, IPv4, or IPv6 traffic (as denoted solely by the EtherType), broadcast frames, or neighbor discovery traffic.

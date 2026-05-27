#!/usr/bin/env bash

#
# setup_tap.sh
#
# Creates and configures TAP interface for tiny-netstack
#
# Usage:
# 	sudo ./scripts/setup_tap.sh
#

set -e

TAP_DEV="tap0"
TAP_IP="10.0.0.1/24"

echo "[+] Creating TAP interface: ${TAP_DEV}"

ip tuntap add dev "${TAP_DEV}" mode tap

echo "[+] Assigning IP address: ${TAP_IP}"

ip addr add "${TAP_IP}" dev "${TAP_DEV}"

echp "[+] Bringing interface up"

ip link set "${TAP_DEV}" up

echo 
echo "[+] TAP interface configured successfully"
echo

ip addr show "${TAP_DEV}"

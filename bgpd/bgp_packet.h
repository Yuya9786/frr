/* BGP packet management header.
 * Copyright (C) 1999 Kunihiro Ishiguro
 *
 * This file is part of GNU Zebra.
 *
 * GNU Zebra is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * GNU Zebra is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; see the file COPYING; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef _QUAGGA_BGP_PACKET_H
#define _QUAGGA_BGP_PACKET_H

#include "hook.h"

DECLARE_HOOK(bgp_packet_dump,
		(struct peer *peer, uint8_t type, bgp_size_t size,
			struct stream *s),
		(peer, type, size, s));

DECLARE_HOOK(bgp_packet_send,
		(struct peer *peer, uint8_t type, bgp_size_t size,
			struct stream *s),
		(peer, type, size, s));

#define BGP_NLRI_LENGTH       1U
#define BGP_TOTAL_ATTR_LEN    2U
#define BGP_UNFEASIBLE_LEN    2U

/* When to refresh */
#define REFRESH_IMMEDIATE 1
#define REFRESH_DEFER     2

/* ORF Common part flag */
#define ORF_COMMON_PART_ADD        0x00
#define ORF_COMMON_PART_REMOVE     0x80
#define ORF_COMMON_PART_REMOVE_ALL 0xC0
#define ORF_COMMON_PART_PERMIT     0x00
#define ORF_COMMON_PART_DENY       0x20

#define BGP_UPDATE_EOR_PKT(_peer, _afi, _safi, _s)                             \
	do {                                                                   \
		_s = bgp_update_packet_eor(_peer, _afi, _safi);                \
		if (_s) {                                                      \
			bgp_packet_add(_peer, _s);                             \
		}                                                              \
	} while (0)

/* Packet send and receive function prototypes. */
extern void bgp_keepalive_send(struct peer *);
extern void bgp_open_send(struct peer *);
extern void bgp_notify_send(struct peer *, uint8_t, uint8_t);
extern void bgp_notify_send_with_data(struct peer *, uint8_t, uint8_t,
				      uint8_t *, size_t);
extern void bgp_route_refresh_send(struct peer *peer, afi_t afi, safi_t safi,
				   uint8_t orf_type, uint8_t when_to_refresh,
				   int remove, uint8_t subtype);
extern void bgp_capability_send(struct peer *, afi_t, safi_t, int, int);

extern int bgp_capability_receive(struct peer *, bgp_size_t);

extern int bgp_nlri_parse(struct peer *, struct attr *, struct bgp_nlri *,
			  int mp_withdraw);

extern void bgp_update_restarted_peers(struct peer *);
extern void bgp_update_implicit_eors(struct peer *);
extern void bgp_check_update_delay(struct bgp *);

extern int bgp_packet_set_marker(struct stream *s, uint8_t type);
extern void bgp_packet_set_size(struct stream *s);

extern void bgp_generate_updgrp_packets(struct thread *);
extern void bgp_process_packet(struct thread *);

extern void bgp_send_delayed_eor(struct bgp *bgp);

/* Task callback to handle socket error encountered in the io pthread */
void bgp_packet_process_error(struct thread *thread);
extern struct bgp_notify
bgp_notify_decapsulate_hard_reset(struct bgp_notify *notify);
extern bool bgp_notify_send_hard_reset(struct peer *peer, uint8_t code,
				       uint8_t subcode);
extern bool bgp_notify_received_hard_reset(struct peer *peer, uint8_t code,
					   uint8_t subcode);

#endif /* _QUAGGA_BGP_PACKET_H */

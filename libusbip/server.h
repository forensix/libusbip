/**
 * libusbip - server.h (Private libusbip header file)
 * Copyright (C) 2011 Manuel Gebele
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "libusbip.h"
#include <libusb-1.0/libusb.h>

libusbip_rpc_t server_read_rpc(int sock);

void server_usb_init(struct libusbip_connection_info *ci, struct libusb_context **ctx);
void server_usb_exit(struct libusb_context *ctx);
void server_usb_get_device_list(struct libusbip_connection_info *ci, struct libusb_context *ctx);
void server_usb_get_device_descriptor(struct libusbip_connection_info *ci, struct libusb_context *ctx);
void server_usb_open(struct libusbip_connection_info *ci, struct libusb_context *ctx);
void server_usb_close(struct libusbip_connection_info *ci);
void server_usb_claim_interface(struct libusbip_connection_info *ci);
void server_usb_open_device_with_vid_pid(struct libusbip_connection_info *ci, struct libusb_context *ctx);
void server_usb_release_interface(struct libusbip_connection_info *ci);
void server_usb_get_configuration(struct libusbip_connection_info *ci);
void server_usb_set_configuration(struct libusbip_connection_info *ci);
void server_usb_set_interface_alt_setting(struct libusbip_connection_info *ci);
void server_usb_reset_device(struct libusbip_connection_info *ci);
void server_usb_clear_halt(struct libusbip_connection_info *ci);
void server_usb_get_string_descriptor_ascii(struct libusbip_connection_info *ci);
void server_usb_control_transfer(struct libusbip_connection_info *ci);
void server_usb_bulk_transfer(struct libusbip_connection_info *ci);
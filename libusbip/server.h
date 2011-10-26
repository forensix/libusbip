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
void server_usb_open(struct libusbip_connection_info *ci, struct libusb_context *ctx, struct libusb_device_handle *dh);
void server_usb_close(struct libusbip_connection_info *ci, struct libusb_device_handle *dh);
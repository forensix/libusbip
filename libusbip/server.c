/**
 * libusbip - server.c
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

#include "server.h"
#include "proto.h"

void
server_usb_init(struct libusbip_connection_info *ci, struct libusb_context **ctx) {
    libusbip_error_t error = libusb_init(ctx);
    
    proto_send_int(&error, ci->client_sock);
}

void
server_usb_exit(struct libusb_context *ctx) {
    libusb_exit(ctx);
}

libusbip_rpc_t
server_read_rpc(int sock) {
    libusbip_rpc_t rpc;
    
    proto_recv_rpc((int *)&rpc, sock);
    
    return rpc;
}
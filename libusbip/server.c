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

#include <strings.h>

#include "libusbi.h"

libusbip_rpc_t
server_read_rpc(int sock) {
    libusbip_rpc_t rpc;
    
    proto_recv_rpc(&rpc, sock);
    
    return rpc;
}

void
server_usb_init(struct libusbip_connection_info *ci, struct libusb_context **ctx) {
    int error = libusb_init(ctx);
    
    proto_send_int(&error, ci->client_sock);
}

void
server_usb_exit(struct libusb_context *ctx) {
    libusb_exit(ctx);
}

void
server_usb_get_device_list(struct libusbip_connection_info *ci, struct libusb_context *ctx) {
    struct libusb_device **list;
    struct libusbip_device_list ilist;
    size_t i;
    ssize_t cnt;
    
    bzero(&ilist, sizeof(struct libusbip_device_list));

    cnt =  libusb_get_device_list(ctx, &list);
    for (i = 0; i < cnt && i < LIBUSBIP_MAX_DEVS; i++) {
        struct libusb_device *dev = list[i];
        struct libusbip_device *idev = &ilist.devices[i];
        idev->bus_number = dev->bus_number;
        idev->device_address = dev->device_address;
        idev->num_configurations = dev->num_configurations;
        idev->session_data = dev->session_data;
        ilist.n_devices++;
    }
    
    proto_send_struct_devlist(&ilist, ci->client_sock);
    
    libusb_free_device_list(list, 1);
}

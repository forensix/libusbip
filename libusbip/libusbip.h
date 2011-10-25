/**
 * libusbip - libusbip.h (Public libusbip header file)
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

#ifndef LIBUSBIP_H
#define LIBUSBIP_H

#include "inttypes.h"

#define LIBUSBIP_MAX_DEVS 256

typedef enum {
    LIBUSBIP_E_SUCCESS,
    LIBUSBIP_E_FAILURE = -1
} libusbip_error_t;

typedef enum {
    LIBUSBIP_RPC_USB_INIT,
    LIBUSBIP_RPC_USB_EXIT
} libusbip_rpc_t;

typedef enum {
    LIBUSBIP_CTX_CLIENT,
    LIBUSBIP_CTX_SERVER
} libusbip_ctx_t;

struct libusbip_device {
    uint16_t bus_number;
    uint16_t device_address;
    uint16_t num_configurations;
    uint16_t session_data;
};

struct libusbip_device_list {
    uint16_t n_devices;
    struct libusbip_device devices[LIBUSBIP_MAX_DEVS];
};

struct libusbip_connection_info {
    int server_sock;
    int client_sock;
};

struct libusbip_rpc_info {
    struct libusbip_connection_info ci;
};

libusbip_error_t libusbip_init(struct libusbip_connection_info *ci, libusbip_ctx_t ctx);
void libusbip_exit(struct libusbip_connection_info *ci, libusbip_ctx_t ctx);
libusbip_rpc_t libusbip_get_rpc(int sock);
libusbip_error_t libusbip_rpc_call(libusbip_rpc_t rpc, libusbip_ctx_t ctx, struct libusbip_rpc_info *ri);

#endif /* LIBUSBIP_H */
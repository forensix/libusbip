/**
 * libusbip - libusbip.c
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
#include "client.h"
#include "server.h"

#include <libusb-1.0/libusb.h>

#define VALID_LIBUSBIP_RPC_INFO(ri) (ri != NULL)

static struct libusb_context *libusbip_ctx = NULL;

libusbip_error_t
libusbip_init(struct libusbip_connection_info *ci, libusbip_ctx_t ctx) {
    libusbip_error_t error = LIBUSBIP_E_SUCCESS;
    
    if (ctx == LIBUSBIP_CTX_CLIENT)
        error = client_usb_init(ci);
    else if (ctx == LIBUSBIP_CTX_SERVER)
        server_usb_init(ci, &libusbip_ctx);
    else
        error = LIBUSBIP_E_FAILURE;
    
    return error;
}

void libusbip_exit(struct libusbip_connection_info *ci, libusbip_ctx_t ctx) {
    if (ctx == LIBUSBIP_CTX_CLIENT)
        client_usb_exit(ci);
    else if (ctx == LIBUSBIP_CTX_SERVER)
        server_usb_exit(libusbip_ctx);
}

libusbip_rpc_t
libusbip_get_rpc(int sock) {
    return server_read_rpc(sock);
}

libusbip_error_t
libusbip_rpc_call(libusbip_rpc_t rpc, libusbip_ctx_t ctx, struct libusbip_rpc_info *ri) {
    libusbip_error_t error = LIBUSBIP_E_SUCCESS;

    if (!VALID_LIBUSBIP_RPC_INFO(ri)) {
        error = LIBUSBIP_E_FAILURE;
        goto done;
    }
    
    switch (rpc) {
    case LIBUSBIP_RPC_USB_INIT:
        error = libusbip_init(&ri->ci, ctx);
        break;
    case LIBUSBIP_RPC_USB_EXIT:
        libusbip_exit(&ri->ci, ctx);
        break;
    default:
        error = LIBUSBIP_E_FAILURE;
        break;
    }
    
done:
    return error;
}
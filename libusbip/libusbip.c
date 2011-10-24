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

#include "client.h"
#include "server.h"

#include <libusb-1.0/libusb.h>

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


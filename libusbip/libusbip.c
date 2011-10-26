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
#include "error.h"

#include <libusb-1.0/libusb.h>

#define IS_VALID_STRUCT(__struct) (__struct != NULL)
#define IS_VALID_CONTEXT(__ctx) (__ctx == LIBUSBIP_CTX_CLIENT || __ctx == LIBUSBIP_CTX_SERVER)

/*
 * TODO:
 * Currently we're using one static handle.  This implies that we've
 * only one client.  If we need to handle more than one client, we
 * need to use libusbip_device_handle session_data to identify the corresponding
 * client!
 */
static struct libusb_device_handle *libusbip_hdl = NULL; 
static struct libusb_context *libusbip_ctx = NULL;

libusbip_rpc_t
libusbip_get_rpc(int sock) {
    return server_read_rpc(sock);
}

libusbip_error_t
libusbip_init(struct libusbip_connection_info *ci, libusbip_ctx_t ctx) {
    libusbip_error_t error = LIBUSBIP_E_SUCCESS;
    
    if (!IS_VALID_STRUCT(ci)) {
        error_illegal_libusbip_connection_info(__func__);
        error = LIBUSBIP_E_FAILURE;
        return error;
    }
    
    if (ctx == LIBUSBIP_CTX_CLIENT)
        error = client_usb_init(ci);
    else if (ctx == LIBUSBIP_CTX_SERVER)
        server_usb_init(ci, &libusbip_ctx);
    else {
        error_illegal_libusbip_ctx_t(__func__);
        error = LIBUSBIP_E_FAILURE;
    }
    
    return error;
}

void
libusbip_exit(struct libusbip_connection_info *ci, libusbip_ctx_t ctx) {    
    if (!IS_VALID_STRUCT(ci)) {
        error_illegal_libusbip_connection_info(__func__);
        return;
    }
    
    if (ctx == LIBUSBIP_CTX_CLIENT)
        client_usb_exit(ci);
    else if (ctx == LIBUSBIP_CTX_SERVER)
        server_usb_exit(libusbip_ctx);
    else
        error_illegal_libusbip_ctx_t(__func__);
}

void
libusbip_get_device_list(struct libusbip_connection_info *ci, libusbip_ctx_t ctx,
                         struct libusbip_device_list *dl) {
    if (!IS_VALID_STRUCT(ci)) {
        error_illegal_libusbip_connection_info(__func__);
        return;
    }
    if (!IS_VALID_STRUCT(dl)) {
        error_illegal_libusbip_device_list(__func__);
        return;
    }
    
    if (ctx == LIBUSBIP_CTX_CLIENT)
        client_usb_get_device_list(ci, dl);
    else if (ctx == LIBUSBIP_CTX_SERVER)
        server_usb_get_device_list(ci, libusbip_ctx);
    else
        error_illegal_libusbip_ctx_t(__func__);
}

libusbip_error_t
libusbip_get_device_descriptor(struct libusbip_connection_info *ci, libusbip_ctx_t ctx,
                               struct libusbip_device *dev,
                               struct libusbip_device_descriptor *dd) {
    libusbip_error_t error = LIBUSBIP_E_SUCCESS;
    
    if (!IS_VALID_STRUCT(ci)) {
        error_illegal_libusbip_connection_info(__func__);
        error = LIBUSBIP_E_FAILURE;
        return error;
    }
    if (!IS_VALID_STRUCT(dev)) {
        error_illegal_libusbip_device(__func__);
        error = LIBUSBIP_E_FAILURE;
        return error;
    }
    if (!IS_VALID_STRUCT(dd)) {
        error_illegal_libusbip_device_descriptor(__func__);
        error = LIBUSBIP_E_FAILURE;
        return error;
    }
    
    if (ctx == LIBUSBIP_CTX_CLIENT)
        error = client_usb_get_device_descriptor(ci, dev, dd);
    else if (ctx == LIBUSBIP_CTX_SERVER)
        server_usb_get_device_descriptor(ci, libusbip_ctx);
    else {
        error_illegal_libusbip_ctx_t(__func__);
        error = LIBUSBIP_E_FAILURE;
    }
    
    return error;
}

libusbip_error_t
libusbip_open(struct libusbip_connection_info *ci, libusbip_ctx_t ctx,
              struct libusbip_device *dev, struct libusbip_device_handle *dh) {
    libusbip_error_t error = LIBUSBIP_E_SUCCESS;

    if (!IS_VALID_STRUCT(ci)) {
        error_illegal_libusbip_connection_info(__func__);
        error = LIBUSBIP_E_FAILURE;
        return error;
    }
    if (!IS_VALID_STRUCT(dev)) {
        error_illegal_libusbip_device(__func__);
        error = LIBUSBIP_E_FAILURE;
        return error;
    }
    if (!IS_VALID_STRUCT(dh)) {
        error_illegal_libusbip_device_handle(__func__);
        error = LIBUSBIP_E_FAILURE;
        return error;
    }
    
    if (ctx == LIBUSBIP_CTX_CLIENT)
        error = client_usb_open(ci, dev, dh);
    else if (ctx == LIBUSBIP_CTX_SERVER)
        server_usb_open(ci, libusbip_ctx, libusbip_hdl);
    else {
        error_illegal_libusbip_ctx_t(__func__);
        error = LIBUSBIP_E_FAILURE;
    }
    
    return error;
}

void
libusbip_close(struct libusbip_connection_info *ci, libusbip_ctx_t ctx,
               struct libusbip_device_handle *dh) {
    if (!IS_VALID_STRUCT(dh)) {
        error_illegal_libusbip_device_handle(__func__);
        return;
    }
    
    if (ctx == LIBUSBIP_CTX_CLIENT)
        client_usb_close(ci, dh);
    else if (ctx == LIBUSBIP_CTX_SERVER)
        server_usb_close(ci, libusbip_hdl);
    else
        error_illegal_libusbip_ctx_t(__func__);
}

libusbip_error_t
libusbip_rpc_call(libusbip_rpc_t rpc, libusbip_ctx_t ctx, struct libusbip_rpc_info *ri) {
    libusbip_error_t error = LIBUSBIP_E_SUCCESS;

    if (!IS_VALID_STRUCT(ri)) {
        error_illegal_libusbip_rpc_info(__func__);
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
    case LIBUSBIP_RPC_USB_GET_DEVICE_LIST:
        libusbip_get_device_list(&ri->ci, ctx, &ri->dl);
        break;
    case LIBUSBIP_RPC_USB_GET_DEVICE_DESCRIPTOR:
        libusbip_get_device_descriptor(&ri->ci, ctx, &ri->dev, &ri->dd);
        break;
    case LIBUSBIP_RPC_USB_OPEN:
        libusbip_open(&ri->ci, ctx, &ri->dev, &ri->dh);
        break;
    case LIBUSBIP_RPC_USB_CLOSE:
        libusbip_close(&ri->ci, ctx, &ri->dh);
        break;
    default:
        error_illegal_libusbip_rpc_t(__func__);
        error = LIBUSBIP_E_FAILURE;
        break;
    }
    
done:
    return error;
}

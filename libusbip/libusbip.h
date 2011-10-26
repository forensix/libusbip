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
    LIBUSBIP_RPC_USB_EXIT,
    LIBUSBIP_RPC_USB_GET_DEVICE_LIST,
    LIBUSBIP_RPC_USB_GET_DEVICE_DESCRIPTOR,
    LIBUSBIP_RPC_USB_OPEN,
    LIBUSBIP_RPC_USB_OPEN_DEVICE_WITH_VID_PID,
    LIBUSBIP_RPC_USB_CLOSE,
    LIBUSBIP_RPC_USB_CLAIM_INTERFACE,
    LIBUSBIP_RPC_USB_RELEASE_INTERFACE,
    LIBUSBIP_RPC_USB_GET_CONFIGURATION,
    LIBUSBIP_RPC_USB_SET_CONFIGURATION,
    LIBUSBIP_RPC_USB_SET_INTERFACE_ALT_SETTING,
    LIBUSBIP_RPC_USB_RESET_DEVICE,
    LIBUSBIP_RPC_USB_CLEAR_HALT,
} libusbip_rpc_t;

typedef enum {
    LIBUSBIP_CTX_CLIENT,
    LIBUSBIP_CTX_SERVER
} libusbip_ctx_t;

struct libusbip_device {
    uint16_t bus_number;
    uint16_t device_address;
    uint16_t num_configurations;
    uint32_t session_data;
};

struct libusbip_device_list {
    int n_devices;
    struct libusbip_device devices[LIBUSBIP_MAX_DEVS];
};

struct libusbip_device_descriptor {
    uint16_t bLength;
    uint16_t bDescriptorType;
    uint16_t bcdUSB;
    uint16_t bDeviceClass;
    uint16_t bDeviceSubClass;
    uint16_t bDeviceProtocol;    
    uint16_t bMaxPacketSize0;    
    uint16_t idVendor;    
    uint16_t idProduct;    
    uint16_t bcdDevice;    
    uint16_t iManufacturer;    
    uint16_t iProduct;    
    uint16_t iSerialNumber;    
    uint16_t bNumConfigurations;
};

struct libusbip_device_handle {
    uint32_t session_data;
};

struct libusbip_connection_info {
    int server_sock;
    int client_sock;
};

struct libusbip_rpc_info {
    struct libusbip_connection_info ci;
    struct libusbip_device_list dl;
    struct libusbip_device dev;
    struct libusbip_device_descriptor dd;
    struct libusbip_device_handle dh;
    
    int intf;
    int conf;
    int alt_setting;
    uint16_t vid;
    uint16_t pid;
    uint16_t endpoint;
};

libusbip_rpc_t libusbip_get_rpc(int sock);

libusbip_error_t libusbip_rpc_call(libusbip_rpc_t rpc, libusbip_ctx_t ctx, struct libusbip_rpc_info *ri);
libusbip_error_t libusbip_init(struct libusbip_connection_info *ci, libusbip_ctx_t ctx);
libusbip_error_t libusbip_get_device_descriptor(struct libusbip_connection_info *ci, libusbip_ctx_t ctx, struct libusbip_device *dev, struct libusbip_device_descriptor *dd);
libusbip_error_t libusbip_open(struct libusbip_connection_info *ci, libusbip_ctx_t ctx, struct libusbip_device *dev, struct libusbip_device_handle *dh);
libusbip_error_t libusbip_claim_interface(struct libusbip_connection_info *ci, libusbip_ctx_t ctx, struct libusbip_device_handle *dh, int intf);
libusbip_error_t libusbip_release_interface(struct libusbip_connection_info *ci, libusbip_ctx_t ctx, struct libusbip_device_handle *dh, int intf);
libusbip_error_t libusbip_get_configuration(struct libusbip_connection_info *ci, libusbip_ctx_t ctx, struct libusbip_device_handle *dh, int conf);
libusbip_error_t libusbip_set_interface_alt_setting(struct libusbip_connection_info *ci, libusbip_ctx_t ctx, struct libusbip_device_handle *dh, int intf, int alt_setting);
libusbip_error_t libusbip_reset_device(struct libusbip_connection_info *ci, libusbip_ctx_t ctx, struct libusbip_device_handle *dh);
libusbip_error_t libusbip_clear_halt(struct libusbip_connection_info *ci, libusbip_ctx_t ctx, struct libusbip_device_handle *dh, uint16_t endpoint);

void libusbip_exit(struct libusbip_connection_info *ci, libusbip_ctx_t ctx);
void libusbip_get_device_list(struct libusbip_connection_info *ci, libusbip_ctx_t ctx, struct libusbip_device_list *dl);
void libusbip_close(struct libusbip_connection_info *ci, libusbip_ctx_t ctx, struct libusbip_device_handle *dh);
void libusbip_open_device_with_vid_pid(struct libusbip_connection_info *ci, libusbip_ctx_t ctx, struct libusbip_device_handle *dh, uint16_t vid, uint16_t pid);

#endif /* LIBUSBIP_H */